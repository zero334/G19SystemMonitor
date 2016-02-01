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
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Bitmap* originalImage;
	int originalImageWidth, originalImageHeight;

	void drawGui(Gdiplus::Bitmap* image, std::vector<std::wstring> &vec);

public:
	Gui(const TCHAR* fileName);
	~Gui();
	bool setLcdBackground(std::vector<std::wstring> &vec);
};