#pragma once
#include <string>
#include <Magick++.h>
namespace AG {
	class Image
	{
	public:
		Image(int width, int height, std::string filename);
		Image(std::string filename);

		void resizeImage(int width, int height);
		void cropImage(int width, int height, int offsetX = 0, int offsetY = 0);

		void saveImage();
		void saveImage(std::string filename);

		void addImage(std::string filename, int x, int y);
		void addImage(Magick::Image image, int x, int y);
		void addImage(Magick::Image image, int x, int y, Magick::CompositeOperator compose);

		void addText(std::string text, int x, int y);

		void addBorder();

		Magick::Image getImage() const {
			return image_;
		};

		int getWidth() { return image_.size().width(); };
		int getHeight() { return image_.size().height(); };

		~Image();

	private:
		Magick::Image image_;
		std::string filename_;
	};
}
