#include "pch.h"
#include "ConsoleManager.h"
#include <cstdio>
#include <iostream>

bool ConsoleManager::s_opened = false;

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
