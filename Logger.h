#pragma once
#include <string>

class Logger
{
public:
    enum class LOG_LEVEL {k_Info, k_Warning, k_Error, k_Msg};

public:
    static void Log(LOG_LEVEL lvl, const char* msg);
    static void Log(const char* msg);

};