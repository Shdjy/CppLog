#include "pch.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <cstdio>
#include <malloc.h>
#include <filesystem>

#pragma comment(lib, "Dbghelp.lib")

namespace fs = std::filesystem;

static const char* GetExceptionCodeString(DWORD code)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
	case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
	default: return "UNKNOWN_EXCEPTION";
	}
}

static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	// 确保日志目录存在
	fs::create_directories("./logs");

	FILE* fp = nullptr;
	fopen_s(&fp, "./logs/crash.log", "a+");
	if (!fp)
		return EXCEPTION_EXECUTE_HANDLER;

	fprintf(fp, "\n========== CRASH ==========\n");

	DWORD code = pExceptionInfo->ExceptionRecord->ExceptionCode;
	void* addr = pExceptionInfo->ExceptionRecord->ExceptionAddress;

	fprintf(fp, "ExceptionCode: 0x%08X (%s)\n", code, GetExceptionCodeString(code));
	fprintf(fp, "ExceptionAddress: 0x%p\n", addr);

	if (code == EXCEPTION_ACCESS_VIOLATION && pExceptionInfo->ExceptionRecord->NumberParameters >= 2)
	{
		fprintf(fp, "AccessViolationType: %s\n",
			pExceptionInfo->ExceptionRecord->ExceptionInformation[0] ? "Write" : "Read");
		fprintf(fp, "AccessViolationAddress: 0x%p\n",
			(void*)pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
	}

	if (pExceptionInfo->ExceptionRecord->NumberParameters > 0)
	{
		fprintf(fp, "ExceptionParameters:\n");
		for (DWORD i = 0; i < pExceptionInfo->ExceptionRecord->NumberParameters; ++i)
		{
			fprintf(fp, "  [%d]: 0x%llX\n", i, pExceptionInfo->ExceptionRecord->ExceptionInformation[i]);
		}
	}

	// 打印堆栈
	void* stack[64] = {};
	USHORT frames = CaptureStackBackTrace(0, 64, stack, nullptr);

	HANDLE process = GetCurrentProcess();
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
	SymInitialize(process, nullptr, TRUE);

	SYMBOL_INFO* symbol = (SYMBOL_INFO*)_alloca(sizeof(SYMBOL_INFO) + 256);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	IMAGEHLP_LINE64 line = {};
	line.SizeOfStruct = sizeof(line);
	DWORD displacement = 0;

	DWORD64 lastAddr = 0;

	fprintf(fp, "StackTrace:\n");
	for (USHORT i = 0; i < frames; i++)
	{
		DWORD64 addr = (DWORD64)stack[i];
		if (addr == lastAddr)
			continue; // 去掉重复
		lastAddr = addr;

		if (SymFromAddr(process, addr, nullptr, symbol))
		{
			const char* moduleName = "<unknown>";
			HMODULE hMod = nullptr;
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)addr, &hMod) && hMod)
			{
				static char modName[MAX_PATH] = {};
				GetModuleFileNameA(hMod, modName, MAX_PATH);
				moduleName = modName;
			}

			if (SymGetLineFromAddr64(process, addr, &displacement, &line))
			{
				fprintf(fp, "%02d: %s!%s - %s:%lu (0x%llX)\n",
					i, moduleName, symbol->Name, line.FileName, line.LineNumber, symbol->Address);
			}
			else
			{
				fprintf(fp, "%02d: %s!%s - 0x%llX\n",
					i, moduleName, symbol->Name, symbol->Address);
			}
		}
		else
		{
			fprintf(fp, "%02d: 0x%llX\n", i, addr);
		}
	}

	fprintf(fp, "========== END CRASH ==========\n");

	fflush(fp);
	fclose(fp);
	SymCleanup(process);

	return EXCEPTION_EXECUTE_HANDLER;
}

// 安装全局崩溃处理器
void InstallCrashHandler()
{
	SetUnhandledExceptionFilter(UnhandledExceptionHandler);
}
