#include "Bitmap.h"
#include <cassert>

Bitmap::Bitmap(int width, int height) 
		: m_width(width), 
		  m_height(height),
		  m_pixels(width * height) { }

const std::vector<Pixel> &Bitmap::getPixels() const noexcept
{
	return m_pixels;
}

std::vector<unsigned char> Bitmap::getRawPixels() const
{
	std::vector<unsigned char> res;
	for (const auto &pixel : m_pixels)
	{
		res.emplace_back(pixel.r);
		res.emplace_back(pixel.g);
		res.emplace_back(pixel.b);
		res.emplace_back(pixel.a);
	}

	return res;
}

int Bitmap::getWidth() const noexcept
{
	return m_width;
}

int Bitmap::getHeight() const noexcept
{
	return m_height;
}

int Bitmap::getChannels() const noexcept
{
	return 4;
}

Pixel Bitmap::getPixel(int x, int y) const
{
	assert(x < m_width && y < m_height && x >= 0 && y >= 0);
	return m_pixels[y * m_width + x];
}

void Bitmap::setPixel(int x, int y, Pixel pixel)
{
	assert(x < m_width && y < m_height && x >= 0 && y >= 0);
	m_pixels[y * m_width + x] = pixel;
}
