#include <Windows.h>
#include <thread>
#include <tchar.h>

#include "LogitechLCDLib.h" 
#include "Gui.h"

#pragma comment(lib, "gdiplus.lib")


#define applicationName L"System Monitor"
#define backgroundPicturePath L"background.jpg"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	if (!LogiLcdInit(applicationName, LOGI_LCD_TYPE_COLOR)) {
		MessageBoxA(0, "Failed to initialize the SDK.", "Error", MB_ICONERROR);
		return 0;
	}
	
	if (!LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {
		// Try it again a few times
		bool logiLcdIsConnected = false;
		for (unsigned short i = 1; i <= 10; ++i) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {
				logiLcdIsConnected = true;
				break;
			}	
		}
		if (!logiLcdIsConnected) {
			MessageBoxA(0, "No Logitech keyboard with COLOR lcd display found", "Error", MB_ICONERROR);
			LogiLcdShutdown();
			return 0;
		}
	}

	if (!std::thread::hardware_concurrency()) {
		MessageBoxA(0, "No CPU core detected!", "Error", MB_ICONERROR);
		LogiLcdShutdown();
		return 0;
	}


	// Init background
	Gui g19LcdBackground(backgroundPicturePath);

	bool lcdSuccess = true;
	do {
		lcdSuccess = g19LcdBackground.setLcdBackground();

		LogiLcdUpdate();
		Sleep(1000);
	} while (lcdSuccess);

	return 1;
}