#include "pch.h"
#include "ConsoleSink.h"

ConsoleSink::ConsoleSink()
{
	ConsoleManager::Open();
}

ConsoleSink::~ConsoleSink()
{
	ConsoleManager::Close();
}

void ConsoleSink::Write(const LogMessage& log)
{
	if (log.level == LogLevel::Fatal)
	{
		FileSink::Write(log);
		FileSink::Flush();
	}
	ConsoleManager::SetColorByLevel(log.level);
	std::cout << ExtractTimeHMS(log.timestamp)
		<< " [" << LogLevelToString(log.level) << "] "
		<< " [" << log.fileName << "] "
		<< "[" << log.threadId << "] "
		<< log.function
		<< "() "
		<< log.line
		<< " : "
		<< log.message
		<< std::endl;
}
