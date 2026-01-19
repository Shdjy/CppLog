#include "pch.h"
#include "LogSink.h"

void LogSink::Flush()
{

}

void LogSink::SetSinkInfo(std::string info)
{
	m_sinkInfo = info;
}

std::string LogSink::ExtractTimeHMS(const std::string& timeStr)
{
	size_t pos = timeStr.find(' ');
	if (pos == std::string::npos)
	{
		return timeStr;
	}

	return timeStr.substr(pos + 1);
}

void LogSink::SetIpPort(std::string ip, uint16_t port)
{
	m_ip = ip;
	m_port = port;
}

