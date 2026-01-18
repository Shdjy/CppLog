#pragma once

#define LOG_TRACE(msg) Logger::Instance().Log(LogLevel::Trace, __func__, msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg) Logger::Instance().Log(LogLevel::Debug, __func__, msg, __FILE__, __LINE__)
#define LOG_INFO(msg)  Logger::Instance().Log(LogLevel::Info,  __func__, msg, __FILE__, __LINE__)
#define LOG_WARN(msg)  Logger::Instance().Log(LogLevel::Warn,  __func__, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::Instance().Log(LogLevel::Error, __func__, msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) Logger::Instance().Log(LogLevel::Fatal, __func__, msg, __FILE__, __LINE__)

#define LOG_SETLEVEL(level) Logger::Instance().SetLogLevel(level)
#define LOG_SETSINK(sink) Logger::Instance().SetSink(sink)
#define LOG_SETUDPSINK(ip, port) Logger::Instance().SetUdpSinkIpPort(ip, port)





