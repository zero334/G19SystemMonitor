#include "Gui.h"
#include "hardware.h"
#include <fstream>

Gui::Gui(const TCHAR* fileName) {

	// Check if file exists
	const std::ifstream image(fileName);
	if (!image.good()) {
		MessageBoxA(0, "Image file does not exist!", "Error", MB_ICONERROR);
		exit(0);
	}

	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, NULL);

	// Load the image. Any of the following formats are supported: BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF
	this->originalImage = Gdiplus::Bitmap::FromFile(fileName, false);
	this->originalImageWidth  = this->originalImage->GetWidth();
	this->originalImageHeight = this->originalImage->GetHeight();
}

Gui::~Gui() {
	if (this->originalImage) {
		delete this->originalImage;
		this->originalImage = NULL;
	}
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
}

bool Gui::setLcdBackground() {

	// Copy the original image into a new one which can be edited.
	Gdiplus::Bitmap* image = this->originalImage->Clone(0, 0, this->originalImageWidth, this->originalImageHeight, PixelFormatDontCare);
	
	if (image == NULL) {
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		return false;
	}

	// Draw the GUI
	guiDrawer.drawCPU(image);
	guiDrawer.drawRAM(image);

	// Get the bitmap handle
	HBITMAP hBitmap = NULL;
	Gdiplus::Status status = image->GetHBITMAP(RGB(0, 0, 0), &hBitmap);
	if (status != Gdiplus::Ok) {
		delete image;
		image = NULL;
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
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
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		DeleteObject(hBitmap);
		ReleaseDC(NULL, hdc);
		MessageBoxA(0, "Make sure to use a 320 by 240 image.", "Image size error", MB_ICONWARNING);
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
	ReleaseDC(NULL, hdc);
	DeleteObject(hBitmap);
	return true;
}