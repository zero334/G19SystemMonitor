#pragma once
#include <Windows.h>
#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include "LogitechLCDLib.h" 
#include <vector>
#include <string>

class Gui {

private:
	const TCHAR* fileName;
	unsigned int coreNumber;
	void drawGui(Gdiplus::Bitmap* image, std::vector<std::wstring> &vec);

public:
	Gui(const TCHAR* fileName, const unsigned int &coreNumber);
	bool SetColorBackgroundFromFile(std::vector<std::wstring> &vec);
};