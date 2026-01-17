#pragma once
#include "LogSink.h"

class ConsoleSink :
    public FileSink
{
public:
    ConsoleSink();
    ~ConsoleSink();

    void Write(const LogMessage& msg) override;

private:

};

