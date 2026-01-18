#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	Sleep(1000);
	if (argc != 4)
	{
		std::cout << "用法:\n";
		std::cout << "LogUdpConsole.exe <Title> <IP> <Port>\n";
		std::cout << "示例:\n";
		std::cout << "LogUdpConsole.exe \"UDP日志\" 0.0.0.0 9000\n";
		getchar();
		return -1;
	}

	// 1. 设置控制台标题（Unicode）
	std::wstring title;
	int len = MultiByteToWideChar(CP_UTF8, 0, argv[1], -1, nullptr, 0);
	title.resize(len);
	MultiByteToWideChar(CP_UTF8, 0, argv[1], -1, &title[0], len);
	SetConsoleTitleW(title.c_str());

	// 2. 解析参数
	std::string ip = argv[2];
	int port = std::stoi(argv[3]);

	// 3. 初始化 Winsock
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "WSAStartup 失败\n";
		return -1;
	}

	// 4. 创建 UDP socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET)
	{
		std::cout << "socket 创建失败\n";
		WSACleanup();
		return -1;
	}

	// 5. 绑定地址
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(static_cast<u_short>(port));

	if (ip == "0.0.0.0")
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	}

	if (bind(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "bind 失败\n";
		closesocket(s);
		WSACleanup();
		return -1;
	}

	std::cout << "UDP 日志监听中: " << ip << ":" << port << std::endl;

	// 6. 接收日志
	char buf[204800];
	while (true)
	{
		int lenRecv = recvfrom(s, buf, sizeof(buf) - 1, 0, nullptr, nullptr);
		if (lenRecv > 0)
		{
			buf[lenRecv] = 0;
			printf("%s\n", buf);
		}
	}

	// 不会执行到这里
	closesocket(s);
	WSACleanup();
	return 0;
}
