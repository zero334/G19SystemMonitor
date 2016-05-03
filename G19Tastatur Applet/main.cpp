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


	if (!LogiLcdInit(applicationName, LOGI_LCD_TYPE_COLOR)) {
		MessageBoxA(0, "Failed to initialize the SDK.", "Error", MB_ICONWARNING);
		return 0;
	}
	
	if (!LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {
		MessageBoxA(0, "No Logitech keyboard with COLOR lcd display found", "Error", MB_ICONWARNING);
		LogiLcdShutdown();
		return 0;
	}

	const unsigned int coreNumber = std::thread::hardware_concurrency();
	if (!coreNumber) {
		MessageBoxA(0, "No CPU core detected!", "Error", MB_ICONWARNING);
		LogiLcdShutdown();
		return 0;
	}


	// Init background
	Gui g19LcdBackground(backgroundPicturePath);

	bool lcdSuccess = true;
	do {
		lcdSuccess = g19LcdBackground.setLcdBackground(Hardware::getCpuLoadInfo(coreNumber));

		LogiLcdUpdate();
		Sleep(1000);
	} while (lcdSuccess);
}