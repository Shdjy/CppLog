#pragma once

#define LOG_TRACE(msg) Logger::Instance().Log(LogLevel::Trace, __func__, msg, __FILE__)
#define LOG_DEBUG(msg) Logger::Instance().Log(LogLevel::Debug, __func__, msg, __FILE__)
#define LOG_INFO(msg)  Logger::Instance().Log(LogLevel::Info,  __func__, msg, __FILE__)
#define LOG_WARN(msg)  Logger::Instance().Log(LogLevel::Warn,  __func__, msg, __FILE__)
#define LOG_ERROR(msg) Logger::Instance().Log(LogLevel::Error, __func__, msg, __FILE__)
#define LOG_FATAL(msg) Logger::Instance().Log(LogLevel::Fatal, __func__, msg, __FILE__)

#define LOG_SETLEVEL(level) Logger::Instance().SetLogLevel(level)

#define CONSOLE_OPEN ConsoleManager::Open()
#define CONSOLE_CLOSE ConsoleManager::Close()



