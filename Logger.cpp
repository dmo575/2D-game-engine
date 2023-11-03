#include "Logger.h"
#include "GlobalMacros.h"
#include <iostream>

void Logger::Log(LOG_LEVEL lvl, const char* msg)
{
#if !DEBUG_LOG_INFO
	if (lvl == LOG_LEVEL::k_Info) return;
#endif // DEBUG_LOG_INFO

#if !DEBUG_LOG_WARNING
	if (lvl == LOG_LEVEL::k_Warning) return;
#endif // DEBUG_LOG_WARNING

#if !DEBUG_LOG_ERROR
	if (lvl == LOG_LEVEL::k_Error) return;
#endif // DEBUG_LOG_ERROR

#if !DEBUG_LOG_MSG
	if (lvl == LOG_LEVEL::k_Msg) return;
#endif // DEBUG_LOG_MSG



    const char* logLevel;

	switch (lvl)
	{
	case Logger::LOG_LEVEL::k_Info:
		logLevel = "[_INFO_]    ";
		break;
	case Logger::LOG_LEVEL::k_Warning:
		logLevel = "[_WARNING_] ";
		break;
	case Logger::LOG_LEVEL::k_Error:
		logLevel = "[_ERROR_]   ";
		break;
	default:
		logLevel = "[_UNKNOWN_] ";
		break;
	case Logger::LOG_LEVEL::k_Msg:
		logLevel = "[_MSG_]     ";
		break;
	}

	std::cout << logLevel << msg << std::endl;
}


void Logger::Log(const char* msg)
{
#if !DEBUG_LOG_MSG
	return;
#endif // DEBUG_LOG_MSG

	std::cout << "[_MSG_]     " << msg << std::endl;
}