#include "Gui.h"
#include "hardware.h"
#include <fstream>

Gui::Gui(const std::wstring &backgroundPicturePath) {

	// Check if file exists
	const std::wifstream image(backgroundPicturePath);
	if (!image.good()) {
		MessageBoxA(0, "Image file does not exist!", "Error", MB_ICONERROR);
		exit(0);
	}

	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, nullptr);

	// Load the image. Any of the following formats are supported: BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF
	this->originalImage = Gdiplus::Bitmap::FromFile(backgroundPicturePath.c_str(), false);
	this->originalImageWidth  = this->originalImage->GetWidth();
	this->originalImageHeight = this->originalImage->GetHeight();
}

Gui::~Gui() {
	if (this->originalImage) {
		delete this->originalImage;
		this->originalImage = nullptr;
	}
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
}

bool Gui::setLcdBackground() {

	// Copy the original image into a new one which can be edited.
	std::unique_ptr<Gdiplus::Bitmap> image(this->originalImage->Clone(0, 0, this->originalImageWidth, this->originalImageHeight, PixelFormatDontCare));

	if (image == nullptr) {
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		return false;
	}

	// Draw the GUI elements
	std::unique_ptr<Gdiplus::Graphics> graphics(Gdiplus::Graphics::FromImage(image.get()));
	this->guiDrawer.drawCPU(graphics);
	this->guiDrawer.drawRAM(graphics);
	this->guiDrawer.drawTime(graphics);
	

	// Get the bitmap handle
	HBITMAP hBitmap = nullptr;
	const Gdiplus::Status status = image->GetHBITMAP(RGB(0, 0, 0), &hBitmap);
	if (status != Gdiplus::Ok) {
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		DeleteObject(hBitmap);
		return false;
	}

	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	const HDC hdc = GetDC(nullptr);
	
	if (GetDIBits(hdc, hBitmap, 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS) != 0) {

		if (LOGI_LCD_COLOR_WIDTH != bitmapInfo.bmiHeader.biWidth || LOGI_LCD_COLOR_HEIGHT != bitmapInfo.bmiHeader.biHeight) {
			Gdiplus::GdiplusShutdown(this->gdiplusToken);
			DeleteObject(hBitmap);
			ReleaseDC(nullptr, hdc);
			MessageBoxA(0, "Make sure to use a 320 by 240 image.", "Image size error", MB_ICONWARNING);
			return false;
		}

		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biHeight = -bitmapInfo.bmiHeader.biHeight; // This value needs to be inverted, or else image will show up upside/down

		BYTE byteBitmap[LOGI_LCD_COLOR_WIDTH * LOGI_LCD_COLOR_HEIGHT * 4]; // We have 32 bits per pixel, or 4 bytes

		// Gets the "bits" from the bitmap and copies them into a buffer which is pointed to by byteBitmap.
		if (GetDIBits(hdc, hBitmap, 0,
									-bitmapInfo.bmiHeader.biHeight, // Height here needs to be positive. Since we made it negative previously, let's reverse it again.
									&byteBitmap, (BITMAPINFO *)&bitmapInfo, DIB_RGB_COLORS) != 0) {
			// Send image to LCD
			LogiLcdColorSetBackground(byteBitmap);
		}
	}

	// Cleanup
	ReleaseDC(nullptr, hdc);
	DeleteObject(hBitmap);
	return true;
}