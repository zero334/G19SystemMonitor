#include "drawGui.h"
#include "hardware.h"
#include "configReader.h"
#include "Cast.h"

drawGui::drawGui() {

	Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, NULL);
	const ConfigReader reader("config.cfg");
	{ // CPU
		// Setting margin top & margin left
		if (reader.keyExists("cpu_margin_top")) {
			this->cpu.marginTop = std::stoi(reader.getValueOfKey<std::string>("cpu_margin_top"));
		} else {
			this->cpu.marginTop = 0;
		}

		if (reader.keyExists("cpu_margin_left")) {
			this->cpu.marginLeft = std::stoi(reader.getValueOfKey<std::string>("cpu_margin_left"));
		} else {
			this->cpu.marginLeft = 0;
		}
		// Setting the elements
		if (reader.keyExists("cpu_font")) {
			const std::wstring font = Cast::toWstring( reader.getValueOfKey<std::string>("cpu_font") );
			this->cpu.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(font.c_str(), 12));
		} else {
			this->cpu.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(L"Arial", 12));
		}

		if (reader.keyExists("cpu_line_color_red") && reader.keyExists("cpu_line_color_green") && reader.keyExists("cpu_line_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("cpu_line_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("cpu_line_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("cpu_line_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->cpu.lineColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(textColor, 8));
		} else {
			this->cpu.lineColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(Gdiplus::Color::Red, 8));
		}

		if (reader.keyExists("cpu_border_color_red") && reader.keyExists("cpu_border_color_green") && reader.keyExists("cpu_border_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("cpu_border_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("cpu_border_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("cpu_border_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->cpu.borderColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(textColor));
		} else {
			this->cpu.borderColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(Gdiplus::Color::Red));
		}

		if (reader.keyExists("cpu_text_color_red") && reader.keyExists("cpu_text_color_green") && reader.keyExists("cpu_text_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("cpu_text_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("cpu_text_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("cpu_text_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->cpu.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(textColor));
		} else {
			this->cpu.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::Red));
		}
	}
}

drawGui::~drawGui() {
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
}


void drawGui::drawCPU(Gdiplus::Bitmap* &image) {

	// Init graphics
	Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(image);

	// Set the alignment
	unsigned char marginTop = this->cpu.marginTop;
	const unsigned char marginLeft = this->cpu.marginLeft;
	const unsigned char horizontalBarsizeStart = marginLeft + 60;

	Gdiplus::PointF origin(marginLeft, 0);
	Gdiplus::Rect rect(horizontalBarsizeStart, 0, 100, 8);

	const std::vector<std::wstring> cpuLoad = Hardware::getCpuLoadInfo(this->cpu.coreNumber);

	for (unsigned short iter = 0; iter < cpuLoad.size(); iter++) {
		// Draw text
		const std::wstring coreLabel = L"Core " + std::to_wstring(1 + iter) + L':';
		origin.Y = marginTop - 10;
		graphics->DrawString(coreLabel.c_str(), coreLabel.length(), this->cpu.font.get(), origin, this->cpu.textColor.get());

		// Draw CPU lines
		const unsigned char horizontalBarsizeEnd = horizontalBarsizeStart + std::stoi(cpuLoad.at(iter)); // 100 == Max cpu load
		graphics->DrawLine(this->cpu.lineColor.get(), horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);

		// Draw border
		rect.Y = marginTop - 5;
		graphics->DrawRectangle(this->cpu.borderColor.get(), rect);

		// Next element
		marginTop += 17;
	}

	delete graphics;
}

void drawGui::drawRAM(Gdiplus::Bitmap* &image) {

	// Init graphics
	Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(image);

	// Create the utilitys for drawing the content
	Gdiplus::Pen penWhite(Gdiplus::Color::White), penRed(Gdiplus::Color::Red, 8);
	const Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 255, 0, 0));
	const Gdiplus::Font arialFont(L"Arial", 12);

	// Set the alignment
	unsigned char marginTop = 190;
	const unsigned char marginLeft = 5;
	const unsigned char horizontalBarsizeStart = marginLeft + 50;


	const unsigned short usedRam = Hardware::getVirtualMemoryCurrentlyUsed();

	// Draw text
#define centerText  100 
	const Gdiplus::PointF origin(marginLeft + centerText, marginTop);
	const std::wstring coreLabel = std::to_wstring(usedRam) + L"% Ram";
	graphics->DrawString(coreLabel.c_str(), coreLabel.length(), &arialFont, origin, &redBrush);

	// Set a new brush
	Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleVertical, Gdiplus::Color(200, 0, 0, 0), Gdiplus::Color(255, 255, 0, 0));
	penRed.SetBrush(&hatchBrush);
#define doubleRamBarLength 2 // Double the size of the ram bar


	// Draw RAM indicator
	marginTop += 30;
	const unsigned short horizontalBarsizeEnd = horizontalBarsizeStart + usedRam * doubleRamBarLength;
	graphics->DrawLine(&penRed, horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);


	// Draw border
	marginTop -= 5;
	graphics->DrawRectangle(&penWhite, horizontalBarsizeStart, marginTop, 100 * doubleRamBarLength, 8);

	delete graphics;
}