#include "hardwareCheck.h"
#include <thread>
#include <Windows.h>



HardwareCheck::HardwareCheck() {
	hardwareIsWorking = testHardware();
}

bool HardwareCheck::hardwareDetectionIsWorking() const {
	return hardwareIsWorking;
}



bool HardwareCheck::checkCpuCoreNumber() const {
	if (!std::thread::hardware_concurrency()) {
		MessageBoxA(0, "No CPU core detected!", "Error", MB_ICONERROR);
		return false;
	}
	return true;
}

bool HardwareCheck::checkCpuLoad() const {
	if (Hardware::getCpuLoadInfo(1).size() == 0) { // Check minimal Core count (1 Core)
		MessageBoxA(0, "Was not able to detect the cpu load.", "Warning", MB_ICONERROR);
		return false;
	}
	return true;
}

bool HardwareCheck::checkRam() const {
	if (Hardware::getVirtualMemoryCurrentlyUsed() == 0) {
		MessageBoxA(0, "Was not able to detect the memory load.", "Warning", MB_ICONERROR);
		return false;
	}
	return true;
}

bool HardwareCheck::testHardware() const {
	return checkCpuCoreNumber() && checkCpuLoad() && checkRam();
}