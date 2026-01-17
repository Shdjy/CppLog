#pragma once
#include <iostream>
#include <thread>

//日志等级
enum class LogLevel
{
	Trace = 0,
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

enum class Sink
{
	FileSink = 0,
	ConsoleSink,
	DebugViewSink,
	UDPSink,
};

//日志消息
struct LogMessage
{
	LogLevel level;
	std::thread::id threadId;
	std::string fileName;
	std::string timestamp;
	std::string function;
	std::string message;
	long line;
};

