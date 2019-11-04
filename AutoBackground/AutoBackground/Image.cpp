#include "Image.h"
#include <Magick++.h>
#include <iostream>

using namespace AG;

Image::Image(int width, int height, std::string filename) : filename_(filename)
{
	std::cout << "Initalizing magick++!\n";
	Magick::InitializeMagick(NULL);	// init magicks
	try {
		image_ = Magick::Image(Magick::Geometry(width,height),Magick::ColorRGB(1.0, 1.0, 1.0));
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
		return;
	}
	std::cout << "Read file!\n";
}

Image::Image(std::string filename) : filename_(filename)
{
	std::cout << "Initalizing magick++!\n";
	Magick::InitializeMagick(NULL);	// init magicks
	try {
		image_.read(filename);
		//std::cout << filename << " width: " << width_ << " height: " << height_ << std::endl;
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
		return;
	}
	std::cout << "Read file!\n";
}

void Image::addBorder()
{
	try {
		image_.frame(image_.size());
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
		return;
	}
}

void Image::cropImage(int width, int height, int offsetX, int offsetY)
{
	try {
		image_.crop(Magick::Geometry(width,height, offsetX, offsetY));
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
		return;
	}
}

void Image::addText(std::string text,int x, int y) {
	try {
		//image_.draw(Magick::DrawablePointSize(800));
		//image_.draw(Magick::DrawableText(x,y,text));
		image_.annotate(text, Magick::Geometry(100,100, x, y));
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
		return;
	}
	std::cout << "Annotated file!\n";
}

void Image::resizeImage(int width, int height) {
	try {
		std::cout << "Resizing image!" << std::endl;
		//image_.crop(Magick::Geometry(width, height, 0, 0));
		image_.resize(Magick::Geometry(width,height,0,0));
		//std::cout << filename_ << " width: " << width_ << " height: " << height_ << std::endl;
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
	std::cout << "Succesfully resized image!" << std::endl;
}

void Image::saveImage() {
	try {
		std::cout << "Saving image!" << std::endl;
		image_.write(filename_);
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
	std::cout << "Succesfully saved image!" << std::endl;
}

void Image::saveImage(std::string filename) {
	try {
		std::cout << "Saving image!" << std::endl;
		image_.write(filename);
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
	std::cout << "Succesfully saved image!" << std::endl;
}

void Image::addImage(std::string filename,int x, int y) {
	try {
		Magick::Image temp;
		temp.read(filename);
		image_.extent(Magick::Geometry());
		image_.composite(temp, Magick::Geometry(x,y));
	} catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
}

void Image::addImage(Magick::Image image, int x, int y) {
	try {
		image_.composite(image,x,y);
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
}

void Image::addImage(Magick::Image image, int x, int y, Magick::CompositeOperator compose)
{
	try {
		image_.composite(image, x, y, compose);
	}
	catch (Magick::Exception &e) {
		std::cout << "Caught exception: " << e.what() << std::endl; ;
	}
}

Image::~Image()
{
}
