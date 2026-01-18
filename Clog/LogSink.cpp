#include "pch.h"
#include "LogSink.h"

void LogSink::Flush()
{

}

void LogSink::SetSinkInfo(std::string info)
{
	m_sinkInfo = info;
}

void LogSink::SetIpPort(std::string ip, uint16_t port)
{
	m_ip = ip;
	m_port = port;
}

