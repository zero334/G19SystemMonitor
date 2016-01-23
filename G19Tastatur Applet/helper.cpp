#include "helper.h"

Gui::Gui(const TCHAR* fileName, const unsigned int &coreNumber) {
	this->fileName = fileName;
	this->coreNumber = coreNumber;
}

void Gui::drawGui(Gdiplus::Bitmap* image, std::vector<std::wstring> &vec) {

	// Init graphics
	Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(image);

	// Create the utilitys for drawing the content
	Gdiplus::Pen penWhite (Gdiplus::Color::White);
	Gdiplus::Pen penRed   (Gdiplus::Color::Red);
	Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 255, 0, 0));
	penRed.SetWidth(8);

	// Set the alignment
	unsigned char marginTop = 15;
	const unsigned char marginLeft = 5;
	const unsigned char horizontalBarsizeStart = marginLeft + 60;
	

	for (unsigned short iter = 0; iter < 8; iter++) {
		// Draw text
		std::wstring coreLabel = L"Core " + std::to_wstring(1 + iter) + L':';
		Gdiplus::Font myFont(L"Arial", 12);
		Gdiplus::PointF origin(marginLeft, marginTop - 10);
		graphics->DrawString(coreLabel.c_str(), coreLabel.length(), &myFont, origin, &redBrush);

		// Draw CPU lines
		unsigned short horizontalBarsizeEnd = horizontalBarsizeStart + std::stoi(vec.at(iter)); // 100 == Max cpu load
		graphics->DrawLine(&penRed, horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);

		// Draw border
		Gdiplus::Rect rect(horizontalBarsizeStart, marginTop - 5, 100, 8);
		graphics->DrawRectangle(&penWhite, rect);

		// Next element
		marginTop += 17;
	}
	delete graphics;
}


bool Gui::SetColorBackgroundFromFile(std::vector<std::wstring> &vec) {

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	

	// Load the image. Any of the following formats are supported: BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(this->fileName, false);

	if (image == NULL) {
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return false;
	}

	// Draw the gui
	this->drawGui(image, vec);

	// Get the bitmap handle
	HBITMAP hBitmap = NULL;
	Gdiplus::Status status = image->GetHBITMAP(RGB(0, 0, 0), &hBitmap);
	if (status != Gdiplus::Ok) {
		delete image;
		image = NULL;
		Gdiplus::GdiplusShutdown(gdiplusToken);
		DeleteObject(hBitmap);
		return false;
	}

	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HDC hdc = GetDC(NULL);

	// Check what we got
	int ret = GetDIBits(hdc, hBitmap, 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS);

	if (LOGI_LCD_COLOR_WIDTH != bitmapInfo.bmiHeader.biWidth || LOGI_LCD_COLOR_HEIGHT != bitmapInfo.bmiHeader.biHeight) {
		delete image;
		image = NULL;
		Gdiplus::GdiplusShutdown(gdiplusToken);
		DeleteObject(hBitmap);
		ReleaseDC(NULL, hdc);
		std::cout << "Oooops. Make sure to use a 320 by 240 image for color background." << std::endl;
		return false;
	}

	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biHeight = -bitmapInfo.bmiHeader.biHeight; // this value needs to be inverted, or else image will show up upside/down

	BYTE byteBitmap[LOGI_LCD_COLOR_WIDTH * LOGI_LCD_COLOR_HEIGHT * 4]; // we have 32 bits per pixel, or 4 bytes

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by byteBitmap.
	ret = GetDIBits(hdc, hBitmap, 0,
		-bitmapInfo.bmiHeader.biHeight, // height here needs to be positive. Since we made it negative previously, let's reverse it again.
		&byteBitmap,
		(BITMAPINFO *)&bitmapInfo, DIB_RGB_COLORS);

	LogiLcdColorSetBackground(byteBitmap); // Send image to LCD

	// Delete the image when done 
	if (image) {
		delete image;
		image = NULL;
	}
	// General cleanup
	Gdiplus::GdiplusShutdown(gdiplusToken);
	ReleaseDC(NULL, hdc);
	DeleteObject(hBitmap);
	return true;
}