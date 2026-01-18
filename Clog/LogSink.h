#pragma once
class LogSink
{
public:
	virtual ~LogSink() = default;

	virtual void Write(const LogMessage& log) = 0;
	virtual void Flush();
	virtual void SetIpPort(std::string ip, uint16_t port); 
	void SetSinkInfo(std::string info);
	
	

protected:
	std::string m_sinkInfo;
	std::string m_ip;
	uint16_t m_port;

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
};
