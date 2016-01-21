#include <iostream>
#include <Windows.h>
#include <gdiplus.h>

#include "LogitechLCDLib.h" 
#include "helper.h"
#include "config.h"
#include "hardware.h"
#pragma comment(lib, "gdiplus.lib")


#include <thread>


#define applicationName L"System Monitor"
#define backgroundPicturePath L"background.jpg"
int main() {


//	getCpuLoadInfo();


	if (LogiLcdInit(applicationName, LOGI_LCD_TYPE_COLOR)) {
		if (LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR)) {

			// Init background
			Gui g19LcdBackground(backgroundPicturePath);
			bool backgroundPictureSuccess = true; 
				if (backgroundPictureSuccess) {
				
				LogiLcdUpdate();
				unsigned int coreNumber = std::thread::hardware_concurrency();
				std::vector<std::wstring> vec;
				
				while (true) {
					vec = getCpuLoadInfo(coreNumber);
					g19LcdBackground.SetColorBackgroundFromFile(vec);

					//for (unsigned short iter = 0; iter < vec.size(); iter++) {
					//	vec.at(iter).insert(0, L"Core " + std::to_wstring(iter) + L": ") += L"%"; // TODO!

					//	wchar_t* wc = const_cast<wchar_t*>(vec[iter].c_str());
					//	// LogiLcdColorSetText(iter, wc, 255, 0, 0);
					//}
					LogiLcdUpdate();
					
					Sleep(1000); // 1 Sekunde
				}

			} else {
				std::cout << "Background image not found!" << std::endl;
				getchar();
				LogiLcdShutdown();
				return 0;
			}

		}
		std::cout << "No Logitech keyboard with COLOR lcd display found" << std::endl;
		getchar();
		LogiLcdShutdown();
		return 0;
	}

	std::cout << "Failed to initialize the SDK." << std::endl;
	return 0;
}