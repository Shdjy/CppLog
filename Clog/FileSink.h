#pragma once
#include "LogSink.h"
#include <fstream>

class FileSink :
    public LogSink
{

public:
    FileSink();
    void Write(const LogMessage& msg) override;
    void Flush() override;

private:
    void OpenLogFileIfNeeded();

    std::ofstream m_file;
    std::string m_currentDate;
};

