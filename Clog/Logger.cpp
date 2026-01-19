#include "pch.h"
#include "Logger.h"

#include <atomic>
#include <chrono>
#include <iomanip>
#include <thread>
#include "blockingconcurrentqueue.h"

#include <windows.h>
#include <DbgHelp.h>


/* ---------------- Impl 定义 ---------------- */

class Logger::Impl
{
public:
	std::atomic<bool> running{ false };
	std::thread worker;

	moodycamel::BlockingConcurrentQueue<LogMessage> queue;

	std::ofstream file;

	Sink m_sinkWay = Sink::FileSink;
	std::string projectName;
	std::string basePath;
	std::string currentDate;

	std::unique_ptr<LogSink> m_logSink;

	LogLevel minLevel{ LogLevel::Info };

public:
	void WorkerThread()
	{
		LogMessage log;

		while (running)
		{
			// 阻塞等待，带超时，方便安全退出
			if (queue.wait_dequeue_timed(log, std::chrono::milliseconds(100)))
			{
				m_logSink->Write(log);

				// 批量取，减少 Flush 次数
				while (queue.try_dequeue(log))
				{
					m_logSink->Write(log);
				}

				m_logSink->Flush();
			}
		}

		// 退出前清空剩余日志
		while (queue.try_dequeue(log))
		{
			m_logSink->Write(log);
		}

		m_logSink->Flush();
	}

	void CleanupOldLogs()
	{
		auto now = std::chrono::system_clock::now();
		fs::path logDir = fs::path(basePath) / projectName;

		if (!fs::exists(logDir))
		{
			return;
		}

		for (const auto& entry : fs::directory_iterator(logDir))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			auto ftime = fs::last_write_time(entry);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				ftime - fs::file_time_type::clock::now()
				+ std::chrono::system_clock::now());

			if (now - sctp > std::chrono::hours(24 * 30))
			{
				fs::remove(entry);
			}
		}
	}

};

/* ---------------- Logger 对外实现 ---------------- */

Logger& Logger::Instance()
{
	static Logger instance;
	return instance;
}

Logger::Logger()
	: m_impl(std::make_unique<Impl>())
{
}

std::string Logger::GetFileName(const char* fileName)
{
	const char* name = strrchr(fileName, '\\');
	return name ? name + 1 : fileName;
}


Logger::~Logger()
{
	Shutdown();
}

void Logger::Init(const std::string& projectName,
	const std::string& basePath,
	LogLevel minLevel)
{
	InstallCrashHandler();
	auto& impl = *m_impl;

	impl.projectName = projectName;
	impl.basePath = basePath;
	impl.minLevel = minLevel;

	impl.CleanupOldLogs();

	impl.running = true;
	impl.worker = std::thread(&Impl::WorkerThread, &impl);

	m_sinkInfo = basePath + "/" + projectName;
	impl.m_logSink = std::make_unique<FileSink>();//reset(new FileSink());
	impl.m_logSink->SetSinkInfo(m_sinkInfo);
}

void Logger::Log(LogLevel level,
	const char* function,
	const std::string& msg,
	const char* fileName,
	long line)
{
	auto& impl = *m_impl;

	if (!impl.running || level < impl.minLevel)
	{
		return;
	}

	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);

	std::tm tm{};
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

	LogMessage log;
	log.level = level;
	log.timestamp = oss.str();
	log.function = function;
	log.message = msg;
	log.threadId = std::this_thread::get_id();
	log.fileName = GetFileName(fileName);//GetModuleNameFromCaller(caller);
	log.line = line;

	impl.queue.enqueue(std::move(log));
}


void Logger::SetLogLevel(LogLevel level)
{
	auto& impl = *m_impl;
	impl.minLevel = level;
}


void Logger::SetSink(Sink sink)
{
	if (m_impl->m_sinkWay == sink)
	{
		return;
	}
	m_impl->m_sinkWay = sink;
	switch (sink)
	{
	case Sink::FileSink:
		m_impl->m_logSink.reset(new FileSink());
		m_impl->m_logSink->SetSinkInfo(m_sinkInfo);
		break;
	case Sink::ConsoleSink:
		m_impl->m_logSink.reset(new ConsoleSink());
		m_impl->m_logSink->SetSinkInfo(m_sinkInfo);
		break;
	case Sink::DebugViewSink:
		m_impl->m_logSink.reset(new DebugViewSink());
		break;
	case Sink::UDPSink:
		m_impl->m_logSink.reset(new UdpSink());
		m_impl->m_logSink->SetSinkInfo(m_sinkInfo);
		m_impl->m_logSink->SetIpPort(m_udpSinkIp, m_port);
		break;
	default:
		break;
	}	
}

void Logger::SetSinkInfo(std::string sinkInfo)
{
	m_sinkInfo = sinkInfo;
}

void Logger::SetUdpSinkIpPort(std::string ip, uint16_t port)
{
	m_udpSinkIp = ip;
	m_port = port;
}

void Logger::Shutdown()
{
	auto& impl = *m_impl;

	if (!impl.running)
	{
		return;
	}

	impl.running = false;

	if (impl.worker.joinable())
	{
		impl.worker.join();
	}
}

