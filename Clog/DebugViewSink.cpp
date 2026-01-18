#include "pch.h"
#include "DebugViewSink.h"

DebugViewSink::DebugViewSink()
{

}

DebugViewSink::~DebugViewSink()
{

}

void DebugViewSink::Write(const LogMessage& log)
{
	std::string out = log.timestamp
		+ " [" + LogLevelToString(log.level) + "] "
		+ " [" + log.fileName + "] "
		+ "[" + ThreadIdToString(log.threadId) + "] "
		+ log.function
		+ "() "
		+ std::to_string(log.line)
		+ " : "
		+ log.message;

	OutputDebugStringA(out.c_str());
}

std::string DebugViewSink::ThreadIdToString(std::thread::id id)
{
	std::ostringstream oss;
	oss << id;
	return oss.str();
}
