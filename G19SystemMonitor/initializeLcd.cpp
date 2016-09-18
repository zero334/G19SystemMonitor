#include "initializeLcd.h"
#include "LogitechLCDLib.h" 
#include <thread>

bool InitializeLcd::initKeyboard() const {
	if (!LogiLcdInit(applicationName, LOGI_LCD_TYPE_COLOR)) {
		MessageBoxA(0, "Failed to initialize the SDK.", "Error", MB_ICONERROR);
		return false;
	}
	return true;
}

bool InitializeLcd::connectKeyboard() const {
	if (!LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {

		// Try it again a few times
		for (unsigned short i = 1; i <= 10; ++i) {
			if (LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {
				return true;
			} else {
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		
		MessageBoxA(0, "No Logitech keyboard with COLOR lcd display found", "Error", MB_ICONERROR);
		LogiLcdShutdown();
		return false;
	}
	return true;
}

bool InitializeLcd::isInitialized() const {
	if (initKeyboard() && connectKeyboard()) {
		return true;
	}
	return false;
}