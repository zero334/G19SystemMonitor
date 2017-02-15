#pragma once
#include "hardware.h"
class HardwareCheck : private Hardware {
public:
	
	HardwareCheck();
	
	bool hardwareDetectionIsWorking() const;

private:

	bool hardwareIsWorking;

	bool testHardware() const;

	bool checkCpuCoreNumber() const;
	bool checkCpuLoad() const;
	bool checkRam() const;
};