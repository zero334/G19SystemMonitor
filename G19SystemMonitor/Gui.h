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
	unsigned short originalImageWidth, originalImageHeight;

	drawGui guiDrawer;
	std::vector<BYTE> byteBitmap;

public:
	explicit Gui(const std::wstring &backgroundPicturePath);
	~Gui();
	bool setLcdBackground();
};