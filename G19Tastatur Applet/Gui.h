#pragma once
#include "LogitechLCDLib.h" 

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

class Gui {

private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Bitmap* originalImage;
	int originalImageWidth, originalImageHeight;

	void drawCPU(Gdiplus::Bitmap* image, std::vector<std::wstring> &vec);
	void drawRAM(Gdiplus::Bitmap* image);

public:
	explicit Gui(const TCHAR* fileName);
	~Gui();
	bool setLcdBackground(std::vector<std::wstring> &vec);
};