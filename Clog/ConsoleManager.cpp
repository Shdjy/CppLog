#include "pch.h"
#include "ConsoleManager.h"
#include <cstdio>
#include <iostream>

bool ConsoleManager::s_opened = false;
void* ConsoleManager::m_hConsole = nullptr;

BOOL WINAPI ConsoleManager::CtrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
	case CTRL_CLOSE_EVENT:
		// 用户点了控制台右上角关闭
		FreeConsole();
		s_opened = false;
		return TRUE;
	case CTRL_SHUTDOWN_EVENT:
		// 阻止进程退出
		FreeConsole();
		s_opened = false;
		return TRUE;
	default:
		return FALSE;
	}
}

void ConsoleManager::DisableCloseButton()
{
	HWND hWnd = GetConsoleWindow();
	if (hWnd == nullptr)
	{
		return;
	}

	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu == nullptr)
	{
		return;
	}

	// 灰掉“关闭”
	EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	// 或者直接移除（更彻底）
	// RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	DrawMenuBar(hWnd);
}

void ConsoleManager::Open()
{
	if (s_opened)
	{
		return;
	}

	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		AllocConsole();
	}

	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	std::cout.clear();
	std::cerr.clear();
	std::cin.clear();

	SetConsoleTitleW(L"Debug Console");

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	s_opened = true;
	DisableCloseButton();
}

void ConsoleManager::Close()
{
	if (!s_opened)
	{
		return;
	}

	HWND hWnd = GetConsoleWindow();
	if (hWnd != nullptr)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	FreeConsole();
	s_opened = false;
}

bool ConsoleManager::IsOpen()
{
	return s_opened;
}

WORD ConsoleManager::GetCurrentColor(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	return info.wAttributes;
}

void ConsoleManager::SetColorByLevel(LogLevel level)
{
	WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	switch (level)
	{
	case LogLevel::Info:
		color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case LogLevel::Warn:
		color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case LogLevel::Error:
	case LogLevel::Fatal:
		color = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	default:
		break;
	}

	SetConsoleTextAttribute(m_hConsole, color);
}
