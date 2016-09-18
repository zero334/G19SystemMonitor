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

	void drawCPU(std::unique_ptr<Gdiplus::Graphics> &graphics);
	void drawRAM(std::unique_ptr<Gdiplus::Graphics> &graphics);
	void drawTime(std::unique_ptr<Gdiplus::Graphics> &graphics);
};