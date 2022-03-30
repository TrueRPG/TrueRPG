#ifndef IMAGE_HELPER_H
#define IMAGE_HELPER_H

#include <vector>

struct Pixel
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

class Bitmap
{
private:
	std::vector<Pixel> m_pixels;
	int m_width;
	int m_height;
	int m_channels;
public:
	Bitmap() = default;
	Bitmap(int width, int height, int channels = 4);

	const std::vector<Pixel> &getPixels() const;
	std::vector<unsigned char> getRawPixels() const;

	int getWidth() const;
	int getHeight() const;
	int getChannels() const;

	Pixel getPixel(int x, int y) const;
	void setPixel(int x, int y, Pixel pixel);
};

#endif