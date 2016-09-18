#include <Windows.h>
#include <thread>

#include "LogitechLCDLib.h" 
#include "Gui.h"
#include "initializeLcd.h"
#include "hardwareCheck.h"

#pragma comment(lib, "gdiplus.lib")


#define backgroundPicturePath L"background.jpg"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	const InitializeLcd lcd;
	if (!lcd.isInitialized()) {
		return 0;
	}

	const HardwareCheck hCheck;
	if (!hCheck.hardwareDetectionIsWorking()) { // Output only a warning. User may not want to use the not working feature.
		MessageBoxA(0, "Error getting the hardware information for all program features!\n"
					   "Some features may not work.", "Warning", MB_ICONWARNING);
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