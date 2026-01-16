#include "pch.h"
#include "FileSink.h"

FileSink::FileSink()
{

}

void FileSink::Write(const LogMessage& log)
{
	OpenLogFileIfNeeded();

	m_file << log.timestamp
		<< " [" << LogLevelToString(log.level) << "] "
		<< " [" << log.fileName << "] "
		<< "[" << log.threadId << "] "
		<< log.function
		<< " : "
		<< log.message
		<< std::endl;

}


void FileSink::Flush()
{
	if (m_file.is_open())
	{
		m_file.flush();
	}
}

void FileSink::OpenLogFileIfNeeded()
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
	if (date == m_currentDate && m_file.is_open())
	{
		return;
	}

	if (m_file.is_open())
	{
		m_file.close();
	}

	m_currentDate = date;
	//fs::create_directories(basePath + "/" + projectName);
	fs::create_directories(m_sinkInfo);

	//std::string path = basePath + "/" + projectName + "/" + date + ".log";
	std::string path = m_sinkInfo + "/" + date + ".log";
	m_file.open(path, std::ios::app);

}
