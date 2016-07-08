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

		unsigned short marginTop;
		unsigned short marginLeft;

		std::unique_ptr<Gdiplus::Font> font;
		std::unique_ptr<Gdiplus::Pen> lineColor, borderColor;
		std::unique_ptr<Gdiplus::SolidBrush> textColor;

		const unsigned short coreNumber = std::thread::hardware_concurrency();
	} cpu;
	
	struct ramDrawingVariables {

		unsigned short marginTop;
		unsigned short marginLeft;
		std::unique_ptr<Gdiplus::PointF> textOrigin; // Coordinates for the text

		std::unique_ptr<Gdiplus::Font> font;
		std::unique_ptr<Gdiplus::Pen> lineColor, borderColor;
		std::unique_ptr<Gdiplus::SolidBrush> textColor;
		std::unique_ptr<Gdiplus::HatchBrush> hatchBrush;
	} ram;

	struct clockDrawingVariables {

		unsigned short marginTop;
		unsigned short marginLeft;

		std::unique_ptr<Gdiplus::Font> font;
		std::unique_ptr<Gdiplus::SolidBrush> textColor;
	} clock;


public:
	drawGui();
	~drawGui();

	void drawCPU(Gdiplus::Bitmap* &image);
	void drawRAM(Gdiplus::Bitmap* &image);
	void drawTime(Gdiplus::Bitmap* &image);
};