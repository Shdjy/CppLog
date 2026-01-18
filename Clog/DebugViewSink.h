#pragma once
#include "LogSink.h"
class DebugViewSink :
    public LogSink
{
public:
    DebugViewSink();
    ~DebugViewSink() override;
    void Write(const LogMessage& log) override;

private:
    std::string ThreadIdToString(std::thread::id id);
};

