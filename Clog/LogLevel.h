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

//日志消息
struct LogMessage
{
	LogLevel level;
	std::thread::id threadId;
	std::string fileName;
	std::string timestamp;
	std::string function;
	std::string message;
};

