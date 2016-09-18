#pragma once
#include "LogitechLCDLib.h" 
#include "drawGui.h"

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

class Gui {

private:
	const Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Bitmap* originalImage;
	int originalImageWidth, originalImageHeight;

	drawGui guiDrawer;

public:
	explicit Gui(const TCHAR* fileName);
	~Gui();
	bool setLcdBackground();
};