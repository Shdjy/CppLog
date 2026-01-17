#pragma once
#include <Windows.h>

class ConsoleManager
{
public:
	static void Open();
	static void Close();
	static bool IsOpen();
	static WORD GetCurrentColor(HANDLE hConsole);
	static void SetColorByLevel(LogLevel level);

private:
	static BOOL WINAPI CtrlHandler(DWORD ctrlType);
	static void DisableCloseButton();

private:
	static bool s_opened;
	static HANDLE m_hConsole;
};


