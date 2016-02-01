#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <thread>
#include <tchar.h>

#include "LogitechLCDLib.h" 
#include "Gui.h"
#include "config.h"
#include "hardware.h"
#pragma comment(lib, "gdiplus.lib")


#define applicationName L"System Monitor"
#define backgroundPicturePath L"background.jpg"

// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
int main() {
	if (LogiLcdInit(applicationName, LOGI_LCD_TYPE_COLOR)) {
		if (LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {

			const unsigned int coreNumber = std::thread::hardware_concurrency();
			if (!coreNumber) {
				std::cout << "No CPU core detected!" << std::endl;
				getchar();
				LogiLcdShutdown();
				return 0;
			}

			// Init background
			Gui g19LcdBackground(backgroundPicturePath);

			while (true) {
				g19LcdBackground.setLcdBackground(Hardware::getCpuLoadInfo(coreNumber)); // TODO Check return value!

				LogiLcdUpdate();
				Sleep(1000);
			}
		}
		std::cout << "No Logitech keyboard with COLOR lcd display found" << std::endl;
		getchar();
		LogiLcdShutdown();
		return 0;
	}

	std::cout << "Failed to initialize the SDK." << std::endl;
	getchar();
	return 0;
}