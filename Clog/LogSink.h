#pragma once
class LogSink
{
public:
	virtual ~LogSink() = default;

	virtual void Write(const LogMessage& msg) = 0;
	virtual void Flush() = 0;

	void SetSinkInfo(std::string info);
	

protected:
	std::string m_sinkInfo;

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

