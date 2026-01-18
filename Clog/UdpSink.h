#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "FileSink.h"

#pragma comment(lib, "ws2_32.lib") // Á´½Ó¿â

class UdpSink :
    public FileSink
{
public: 
    UdpSink();
    ~UdpSink();
    void Write(const LogMessage& log) override;
    void SetIpPort(std::string ip, uint16_t port) override;

private:

    void InitSocket();
   
	SOCKET m_socket;
	sockaddr_in m_remoteAddr;
	bool m_valid;
};

