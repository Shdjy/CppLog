#pragma once
#include <Windows.h>

class CLOG_API ConsoleManager
{
public:
	static void Open();
	static void Close();
	static bool IsOpen();

private:
	static BOOL WINAPI CtrlHandler(DWORD ctrlType);
	static void DisableCloseButton();

private:
	static bool s_opened;
};


