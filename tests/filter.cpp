#include <png++/png.hpp>
#include <math.h>

#include "filter.h"

GaussFilter::GaussFilter(uint8_t n, float sigma)
	: m_n(n)
{
	m_data.resize(n * n);
	float mean = n/2;
	double sum;
	for (int x = 0; x < n; ++x)
	{
		for (int y = 0; y < n; ++y)
		{
			float v = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
					/ (2 * M_PI * sigma * sigma);
			m_data[x * m_n + y] = v;
			sum += v;
		}
	}

	for (float & v : m_data)
	{
		v /= sum;
	}
}

void GaussFilter::print()
{
	for (int x = 0; x < m_n; ++x)
	{
		for (int y = 0; y < m_n; ++y)
		{
			std::cout << m_data[x * m_n + y] << " ";
		}
		std::cout << "\n";
	}
}



void GaussFilter::filterImage(std::vector<float> &image, int rows, int cols)
{
//	int c = m_n / 2;

	for (int x = 0; x < rows; ++x)
	{
		for (int y = 0; y < cols; ++y)
		{
			for (int xx = 0; xx < m_n; ++xx)
			{
				for (int yy = 0; yy < m_n; ++yy)
				{
					float & v = image[(x + xx) * rows + (y + yy)];
					v = v * m_data[xx * m_n + yy];
				}
			}
		}
	}

}

void GaussFilter::savePNG(const std::string &str, const std::vector<float> &weights, int rows, int cols)
{
	png::image<png::ga_pixel> image;
	image.resize(rows, cols);

	int p = 0;
	for (const float& w : weights)
	{
		unsigned char c = w * 255;
		int row = p / rows;
		int col = p % cols;
		image.set_pixel(col, row, c);
		++p;
	}
	image.write(str);
}
