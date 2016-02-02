#include "Gui.h"
#include "hardware.h"

Gui::Gui(const TCHAR* fileName) {

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

void Gui::drawGui(Gdiplus::Bitmap* image, std::vector<std::wstring> &vec) {

	// Init graphics
	Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(image);

	// Create the utilitys for drawing the content
	Gdiplus::Pen penWhite (Gdiplus::Color::White);
	Gdiplus::Pen penRed   (Gdiplus::Color::Red);
	penRed.SetWidth(8);
	Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 255, 0, 0));
	Gdiplus::Font arialFont(L"Arial", 12);
	
	// Set the alignment
	unsigned char marginTop = 15;
	const unsigned char marginLeft = 5;
	const unsigned char horizontalBarsizeStart = marginLeft + 60;

	Gdiplus::PointF origin(marginLeft, 0);
	Gdiplus::Rect rect(horizontalBarsizeStart, 0, 100, 8);

	for (unsigned short iter = 0; iter < vec.size(); iter++) {
		// Draw text
		std::wstring coreLabel = L"Core " + std::to_wstring(1 + iter) + L':';
		origin.Y = marginTop - 10;
		graphics->DrawString(coreLabel.c_str(), coreLabel.length(), &arialFont, origin, &redBrush);

		// Draw CPU lines
		unsigned short horizontalBarsizeEnd = horizontalBarsizeStart + std::stoi(vec.at(iter)); // 100 == Max cpu load
		graphics->DrawLine(&penRed, horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);

		// Draw border
		rect.Y = marginTop - 5;
		graphics->DrawRectangle(&penWhite, rect);

		// Next element
		marginTop += 17;
	}

	// RAM

	// Draw ram 200px lower on the screen
	marginTop = 200;

	unsigned short usedRam = Hardware::getVirtualMemoryCurrentlyUsed();

	// Draw text
	std::wstring coreLabel = std::to_wstring(usedRam) + L"% Ram";
	origin.Y = marginTop - 10;
	graphics->DrawString(coreLabel.c_str(), coreLabel.length(), &arialFont, origin, &redBrush);

	// Set a new brush
	Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleVertical, Gdiplus::Color(200, 0, 0, 0), Gdiplus::Color(255, 255, 0, 0));
	penRed.SetBrush(&hatchBrush);
	#define doubleRamBarLength 2 // Double the size of the ram bar

	// Draw RAM line
	unsigned short horizontalBarsizeEnd = horizontalBarsizeStart + usedRam * doubleRamBarLength;
	graphics->DrawLine(&penRed, horizontalBarsizeStart, marginTop, horizontalBarsizeEnd, marginTop);

	// Draw border
	rect.Y = marginTop - 5;
	rect.Width = 100 * doubleRamBarLength;
	graphics->DrawRectangle(&penWhite, rect);

	delete graphics;
}


bool Gui::setLcdBackground(std::vector<std::wstring> &vec) {

	// Copy the original image into a new one which can be edited.
	Gdiplus::Bitmap* image = this->originalImage->Clone(0, 0, this->originalImageWidth, this->originalImageHeight, PixelFormatDontCare);
	
	if (image == NULL) {
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
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
	ReleaseDC(NULL, hdc);
	DeleteObject(hBitmap);
	return true;
}