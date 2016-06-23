#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <memory>
#include <thread>

class drawGui {

private:

	const Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	struct cpuDrawingVariables {

		unsigned char marginTop;
		unsigned char marginLeft;
		unsigned char horizontalBarsizeStart;

		std::unique_ptr<Gdiplus::Font> font;
		std::unique_ptr<Gdiplus::Pen> lineColor, borderColor;
		std::unique_ptr<Gdiplus::SolidBrush> textColor;

		const unsigned short coreNumber = std::thread::hardware_concurrency();
	} cpu;
	
	struct ramDrawingVariables {
	// TODO
	} ram;



public:
	drawGui();
	~drawGui();

	void drawCPU(Gdiplus::Bitmap* &image);
	void drawRAM(Gdiplus::Bitmap* &image);
};