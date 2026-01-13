#include "pch.h"
#include "Logger.h"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <thread>

#include "concurrentqueue.h"

namespace fs = std::filesystem;


/* ---------------- Impl 定义 ---------------- */

class Logger::Impl
{
public:
	std::atomic<bool> running{ false };
	std::thread worker;

	moodycamel::ConcurrentQueue<LogMessage> queue;

	std::ofstream file;

	std::string projectName;
	std::string basePath;
	std::string currentDate;

	LogLevel minLevel{ LogLevel::Info };

public:
	inline const char* LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Trace:
			return "TRACE";
		case LogLevel::Debug:
			return "DEBUG";
		case LogLevel::Info:
			return "INFO";
		case LogLevel::Warn:
			return "WARN";
		case LogLevel::Error:
			return "ERROR";
		case LogLevel::Fatal:
			return "FATAL";
		default:
			return "UNKNOWN";
		}
	}

	void WorkerThread()
	{
		while (running)
		{
			bool hasData = false;
			LogMessage log;

			while (queue.try_dequeue(log))
			{
				hasData = true;
				OpenLogFileIfNeeded();

				file << log.timestamp
					<< " [" << LogLevelToString(log.level) << "] "
					<< "[" << log.threadId << "] "
					<< log.function
					<< " : "
					<< log.message
					<< std::endl;
			}

			if (hasData)
			{
				file.flush();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		}

		// 退出前清空残留日志
		LogMessage log;
		while (queue.try_dequeue(log))
		{
			OpenLogFileIfNeeded();
			file << log.timestamp
				<< " [" << LogLevelToString(log.level) << "] "
				<< "[" << log.threadId << "] "
				<< log.function
				<< " : "
				<< log.message
				<< std::endl;
		}

		if (file.is_open())
		{
			file.flush();
		}
	}

	void OpenLogFileIfNeeded()
	{
		auto now = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
#ifdef _WIN32
		localtime_s(&tm, &t);
#else
		localtime_r(&t, &tm);
#endif

		std::ostringstream oss;
		oss << std::put_time(&tm, "%Y-%m-%d");

		std::string date = oss.str();
		if (date == currentDate && file.is_open())
		{
			return;
		}

		if (file.is_open())
		{
			file.close();
		}

		currentDate = date;
		fs::create_directories(basePath + "/" + projectName);

		std::string path = basePath + "/" + projectName + "/" + date + ".log";
		file.open(path, std::ios::app);
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

Logger::~Logger()
{
	Shutdown();
}

void Logger::Init(const std::string& projectName,
	const std::string& basePath,
	LogLevel minLevel)
{
	auto& impl = *m_impl;

	impl.projectName = projectName;
	impl.basePath = basePath;
	impl.minLevel = minLevel;

	impl.CleanupOldLogs();

	impl.running = true;
	impl.worker = std::thread(&Impl::WorkerThread, &impl);
}

void Logger::Log(LogLevel level,
	const char* function,
	const std::string& msg)
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

	impl.queue.enqueue(std::move(log));
}

void Logger::SetLogLevel(LogLevel level)
{
	auto& impl = *m_impl;
	impl.minLevel = level;
}

void Logger::Flush()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
