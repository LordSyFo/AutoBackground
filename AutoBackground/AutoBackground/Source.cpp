#include <iostream>
#include <Windows.h>
#include "Request.h"
#include "Image.h"
#include "ITimeUnit.h"
#include "Time.h"

using namespace AG;

int main() {

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string buf(buffer);
	std::string tmp = buf.substr(0, buf.rfind("\\", buf.rfind("\\")));	// this is getting ridic lmao
	std::string dir = tmp.substr(0, tmp.rfind("\\")+1);

	std::cout << dir;

	Request *reqHandler = new Request();

	std::string cam1time = reqHandler->downloadNewestImage(22,dir + "cam1.jpeg",0);
	std::string cam2time = reqHandler->downloadNewestImage(23, dir + "cam2.jpeg",0);
	std::string cam3time = reqHandler->downloadNewestImage(24, dir + "cam3.jpeg", 0);

	Image *cam1Image = new Image(dir + "cam1.jpeg");
	Image *cam2Image = new Image(dir + "cam2.jpeg");
	Image *cam3Image = new Image(dir + "cam3.jpeg");
	Image *wallpaper = new Image(1920,1080,dir + "wallpaper.jpeg");
	
	/* Make timestamps */
	Image *timestamp1 = new Image(dir + "timestamp_template.jpg");
	timestamp1->addText(cam1time, 76, 3);
	//timestamp1->saveImage(dir + "timestamp.jpg");
	Image *timestamp2 = new Image(dir + "timestamp_template.jpg");
	timestamp2->addText(cam2time, 76, 3);
	//timestamp2->saveImage(dir + "timestamp2.jpg");
	Image *timestamp3 = new Image(dir + "timestamp_template.jpg");
	timestamp3->addText(cam3time, 76, 3);
	//timestamp2->saveImage(dir + "timestamp3.jpg");

	/* Compose cam images */
	cam1Image->resizeImage(1920, 1080);
	cam1Image->addImage(timestamp1->getImage(), cam1Image->getWidth()-timestamp1->getWidth()-5, 5);
	cam1Image->saveImage();
	cam2Image->resizeImage(1920, 1080);
	cam2Image->addImage(timestamp2->getImage(), cam2Image->getWidth()-timestamp2->getWidth()-5, 5);
	cam2Image->saveImage();
	cam3Image->resizeImage(1920, 1080);
	cam3Image->cropImage(1920,190,0, (cam3Image->getHeight() / 2)-30);
	cam3Image->addImage(timestamp3->getImage(), cam3Image->getWidth() - timestamp3->getWidth() - 5, 5);
	cam3Image->saveImage();

	/* Make weather image */
	weatherInfo weather = reqHandler->getTodaysWeather();
	Image *weatherImg = new Image(dir + "weather_template.png");
	weatherImg->addText(std::to_string(weather.temperatureSummit) + " C",104,51);
	weatherImg->addText(std::to_string(weather.snowDepthSummit) + " cm", 120, 88);
	weatherImg->addText(std::to_string(weather.temperatureMiddle) + " C", 104, 152);
	weatherImg->addText(std::to_string(weather.snowDepthMiddle) + " cm", 120, 188);
	weatherImg->addText(std::to_string(weather.temperatureValley) + " C", 104, 247);
	weatherImg->addText(std::to_string(weather.snowDepthValley) + " cm", 120, 282);
	weatherImg->saveImage(dir+"weather.png");

	//wallpaper->addImage(camImage->getImage(),0,0);
	wallpaper->addImage(cam1Image->getImage(),0,0);
	wallpaper->addImage(cam2Image->getImage(),0, cam1Image->getHeight());
	wallpaper->addImage(cam3Image->getImage(), 0, cam1Image->getHeight() + cam2Image->getHeight());
	wallpaper->addImage(weatherImg->getImage(), wallpaper->getWidth()-weatherImg->getWidth()-25, 480, Magick::CompositeOperator::InCompositeOp);
	wallpaper->saveImage();

	int return_value = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID*)(std::string(dir+"wallpaper.jpeg").c_str()), SPIF_UPDATEINIFILE);

	if (return_value) {
		std::cout << "Succesfully changed background image to newest Bad Gastein cam!" << std::endl;
	}
	else {
		std::cout << "Something went wrong while changing the background!" << std::endl;
	}

	return 1;
}