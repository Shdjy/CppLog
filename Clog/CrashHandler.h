#pragma once

#ifdef CLOG_EXPORTS
#define CLOG_API __declspec(dllexport)
#else
#define CLOG_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	CLOG_API void InstallCrashHandler();

#ifdef __cplusplus
}
#endif
#pragma once
