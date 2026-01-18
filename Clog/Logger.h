#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)

#ifdef CLOG_EXPORTS
#define CLOG_API __declspec(dllexport)
#else
#define CLOG_API __declspec(dllimport)
#endif

#include <string>
#include <memory>
#include "LogLevel.h"

class CLOG_API Logger
{
public:
	static Logger& Instance();

	void Init(const std::string& projectName,
		const std::string& basePath,
		LogLevel minLevel);

	void Log(LogLevel level,
		const char* function,
		const std::string& msg,
		const char* fileName,
		long line);

	void SetLogLevel(LogLevel level);
	void SetSink(Sink sink);
	void SetSinkInfo(std::string sinkInfo);
	void SetUdpSinkIpPort(std::string ip, uint16_t port);

	void Flush();
	void Shutdown();

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	std::string GetFileName(const char* fileName);

	std::string m_sinkInfo;
	std::string m_udpSinkIp;
	uint16_t m_port;
private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};
