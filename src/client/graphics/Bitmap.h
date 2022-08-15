#ifndef IMAGE_HELPER_H
#define IMAGE_HELPER_H

#include <vector>

struct Pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

/**
 * @brief Class for manipulate bitmap from pixels
 * 
 */
class Bitmap
{
private:
	std::vector<Pixel> m_pixels;
	int m_width;
	int m_height;
public:
	Bitmap() = default;
	Bitmap(int width, int height);

	/**
	 * @brief Get the pixel structure vector of pixels
	 * 
	 * @return const std::vector<Pixel>& 
	 */
	const std::vector<Pixel> &getPixels() const noexcept;

	/**
	 * @brief Get the byte vector of pixels
	 * 
	 * @return std::vector<unsigned char> 
	 */
	std::vector<unsigned char> getRawPixels() const;

	/**
	 * @brief Get the image width
	 * 
	 * @return int 
	 */
	int getWidth() const noexcept;

	/**
	 * @brief Get the image height
	 * 
	 * @return int 
	 */
	int getHeight() const noexcept;

	/**
	 * @brief Get the image channels
	 * 
	 * @return int 
	 */
	int getChannels() const noexcept;

	/**
	 * @brief Get pixel of the image
	 * 
	 * @param x 
	 * @param y 
	 * @return Pixel 
	 */
	Pixel getPixel(int x, int y) const;

	/**
	 * @brief Set pixel of the image
	 * 
	 * @param x pixel coordinate by width
	 * @param y pixel coordinate by height
	 * @param pixel new pixel color
	 */
	void setPixel(int x, int y, Pixel pixel);
};

#endif