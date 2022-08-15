#pragma once
class InitializeLcd {

#define applicationName L"System Monitor"

	bool initKeyboard() const;
	bool connectKeyboard() const;

public:
	InitializeLcd();
	bool isInitialized() const;
};