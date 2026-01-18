#include "pch.h"
#include "UdpSink.h"

UdpSink::UdpSink()
	: m_socket(INVALID_SOCKET)
	, m_valid(false)
{

}

UdpSink::~UdpSink()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		WSACleanup();
	}
}

void UdpSink::Write(const LogMessage& log)
{
	FileSink::Write(log);
	//UdpSink TODO
	if (!m_valid)
	{
		return;
	}

	std::ostringstream oss;

	oss << log.timestamp
		<< " [" << LogLevelToString(log.level) << "] "
		<< " [" << log.fileName << "] "
		<< "[" << log.threadId << "] "
		<< log.function
		<< "() "
		<< log.line
		<< " : "
		<< log.message;

	std::string data = oss.str();

	sendto(
		m_socket,
		data.c_str(),
		static_cast<int>(data.size()),
		0,
		reinterpret_cast<sockaddr*>(&m_remoteAddr),
		sizeof(m_remoteAddr)
	);
}

void UdpSink::SetIpPort(std::string ip, uint16_t port)
{
	LogSink::SetIpPort(ip, port);
	InitSocket();
}

void UdpSink::InitSocket()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}

	memset(&m_remoteAddr, 0, sizeof(m_remoteAddr));
	m_remoteAddr.sin_family = AF_INET;
	m_remoteAddr.sin_port = htons(m_port);

	if (inet_pton(AF_INET, m_ip.c_str(), &m_remoteAddr.sin_addr) != 1)
	{
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	m_valid = true;
}
