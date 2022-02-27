#include "drawGui.h"
#include "hardware.h"
#include "configReader.h"
#include "Cast.h"
#include "Constants.h"

drawGui::drawGui() {

	Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, nullptr);
	const ConfigReader reader(CONFIG_PATH);
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
	} /// CPU

	{ // RAM
		// Setting margin top, margin left and the start point for the text
		if (reader.keyExists("ram_margin_top") && reader.keyExists("ram_margin_left")) {
			this->ram.marginTop = std::stoi(reader.getValueOfKey<std::string>("ram_margin_top"));
			this->ram.marginLeft = std::stoi(reader.getValueOfKey<std::string>("ram_margin_left"));
		} else {
			this->ram.marginTop = 0;
			this->ram.marginLeft = 0;
		}

		const unsigned short centerText = 100; // Offset to center the text
		this->ram.textOrigin = std::unique_ptr<Gdiplus::PointF>(new Gdiplus::PointF(static_cast<Gdiplus::REAL>(this->ram.marginLeft + centerText), this->ram.marginTop));

		// Setting the elements
		if (reader.keyExists("ram_font")) {
			const std::wstring font = Cast::toWstring(reader.getValueOfKey<std::string>("ram_font"));
			this->ram.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(font.c_str(), 12));
		} else {
			this->ram.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(L"Arial", 12));
		}

		if (reader.keyExists("ram_line_color_red") && reader.keyExists("ram_line_color_green") && reader.keyExists("ram_line_color_blue") &&
			reader.keyExists("ram_line_seperator_color_red") && reader.keyExists("ram_line_seperator_color_green") && reader.keyExists("ram_line_seperator_color_blue") && reader.keyExists("ram_line_seperator_color_alpha")) {

			const int lineRed = std::stoi(reader.getValueOfKey<std::string>("ram_line_color_red"));
			const int lineGreen = std::stoi(reader.getValueOfKey<std::string>("ram_line_color_green"));
			const int lineBlue = std::stoi(reader.getValueOfKey<std::string>("ram_line_color_blue"));

			const Gdiplus::Color lineColor(lineRed, lineGreen, lineBlue);

			const int lineRedSeperator = std::stoi(reader.getValueOfKey<std::string>("ram_line_seperator_color_red"));
			const int lineGreenSeperator = std::stoi(reader.getValueOfKey<std::string>("ram_line_seperator_color_green"));
			const int lineBlueSeperator = std::stoi(reader.getValueOfKey<std::string>("ram_line_seperator_color_blue"));
			const int lineAlphaSeperator = std::stoi(reader.getValueOfKey<std::string>("ram_line_seperator_color_alpha"));

			const Gdiplus::Color lineColorSeperator(lineAlphaSeperator, lineRedSeperator, lineGreenSeperator, lineBlueSeperator);

			this->ram.lineColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(lineColor, 8));
			
			this->ram.hatchBrush = std::unique_ptr<Gdiplus::HatchBrush>(new Gdiplus::HatchBrush(Gdiplus::HatchStyleVertical, lineColorSeperator, lineColor));
			ram.lineColor->SetBrush(this->ram.hatchBrush.get());
		} else {
			this->ram.lineColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(Gdiplus::Color::Red, 8));
		}

		if (reader.keyExists("ram_border_color_red") && reader.keyExists("ram_border_color_green") && reader.keyExists("ram_border_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("ram_border_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("ram_border_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("ram_border_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->ram.borderColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(textColor));
		} else {
			this->ram.borderColor = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(Gdiplus::Color::Red));
		}

		if (reader.keyExists("ram_text_color_red") && reader.keyExists("ram_text_color_green") && reader.keyExists("ram_text_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("ram_text_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("ram_text_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("ram_text_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->ram.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(textColor));
		} else {
			this->ram.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::Red));
		}
	} /// RAM

	{ // Clock
		if (reader.keyExists("clock_margin_top") && reader.keyExists("clock_margin_left")) {
			this->clock.marginTop = std::stoi(reader.getValueOfKey<std::string>("clock_margin_top"));
			this->clock.marginLeft = std::stoi(reader.getValueOfKey<std::string>("clock_margin_left"));
		} else {
			this->clock.marginTop = 0;
			this->clock.marginLeft = 0;
		}

		if (reader.keyExists("clock_font")) {
			const std::wstring font = Cast::toWstring(reader.getValueOfKey<std::string>("clock_font"));
			this->clock.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(font.c_str(), 12));
		} else {
			this->clock.font = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(L"Arial", 12));
		}

		if (reader.keyExists("clock_text_color_red") && reader.keyExists("clock_text_color_green") && reader.keyExists("clock_text_color_blue")) {
			const int red = std::stoi(reader.getValueOfKey<std::string>("clock_text_color_red"));
			const int green = std::stoi(reader.getValueOfKey<std::string>("clock_text_color_green"));
			const int blue = std::stoi(reader.getValueOfKey<std::string>("clock_text_color_blue"));
			const Gdiplus::Color textColor(red, green, blue);
			this->clock.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(textColor));
		} else {
			this->clock.textColor = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::Red));
		}
	} /// Clock
}

drawGui::~drawGui() {
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
}


void drawGui::drawCPU(std::unique_ptr<Gdiplus::Graphics> &graphics) {

	// Set the alignment
	unsigned short marginTop = this->cpu.marginTop;
	const unsigned short marginLeft = this->cpu.marginLeft;
	const unsigned short horizontalBarsizeStart = marginLeft + 60;

	Gdiplus::PointF origin(marginLeft, 0);
	Gdiplus::Rect rect(horizontalBarsizeStart, 0, 100, 8);

	const std::vector<std::wstring> cpuLoad = Hardware::getCpuLoadInfo(this->cpu.coreNumber);

	for (unsigned short iter = 0; iter < cpuLoad.size(); ++iter) {
		// Draw text
		const std::wstring coreLabel = L"Core " + std::to_wstring(1 + iter) + L':';
		origin.Y = static_cast<Gdiplus::REAL>(marginTop - 10);
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
}

void drawGui::drawRAM(std::unique_ptr<Gdiplus::Graphics> &graphics) {

	// Set the alignment
	unsigned short marginTop = this->ram.marginTop;
	const unsigned short marginLeft = this->ram.marginLeft;
	const unsigned short horizontalBarsizeStart = marginLeft + 50;

	const unsigned short usedRam = Hardware::getVirtualMemoryCurrentlyUsed();

	// Draw text
	const std::wstring coreLabel = std::to_wstring(usedRam) + L"% Ram";
	graphics->DrawString(coreLabel.c_str(), coreLabel.length(), this->ram.font.get(), *this->ram.textOrigin, this->ram.textColor.get());


	constexpr unsigned char doubleRamBarLength = 2; // Double the size of the ram bar


	// Draw RAM indicator
	marginTop += 30;
	const unsigned short horizontalBarsizeEnd = horizontalBarsizeStart + usedRam * doubleRamBarLength;
	graphics->DrawLine(this->ram.lineColor.get(), horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);


	// Draw border
	marginTop -= 5;
	graphics->DrawRectangle(this->ram.borderColor.get(), horizontalBarsizeStart, marginTop, 100 * doubleRamBarLength, 8);
}

void drawGui::drawTime(std::unique_ptr<Gdiplus::Graphics> &graphics) {

	const time_t time_now = time(0);   // get time now
	struct tm now;
	localtime_s(&now, &time_now);

	std::wstring hour = std::to_wstring(now.tm_hour);
	if (hour.length() == 1) {
		hour.insert(0, 1, L'0');
	}

	std::wstring minute = std::to_wstring(now.tm_min);
	if (minute.length() == 1) {
		minute.insert(0, 1, L'0');
	}

	const std::wstring time = hour + L':' + minute;
    const Gdiplus::PointF origin(this->clock.marginTop, this->clock.marginLeft);

	graphics->DrawString(time.c_str(), time.length(), this->clock.font.get(), origin, this->clock.textColor.get());
}