#pragma once
#include <Windows.h>
#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include "LogitechLCDLib.h" 


class Gui {

private:
	const TCHAR* fileName;
	Gdiplus::Bitmap* image;

	void drawGui(Gdiplus::Bitmap* image);

public:
	Gui(const TCHAR* fileName);
	~Gui();
	bool SetColorBackgroundFromFile();
};