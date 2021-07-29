#pragma once
#include <windows.h>
#undef GetObject
#include <string>

class OpenFiles
{
public:
	std::string OpenDialog();
	OPENFILENAMEA ofn;       // common dialog box structure
	HWND hwnd;              // owner window
	HANDLE hf;              // file handle
};