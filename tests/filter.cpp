#include <math.h>
#include <string>
#include <stdexcept>

#include <png++/png.hpp>

#include "filter.h"

GaussFilter::GaussFilter(int n, float sigma)
	: m_n(n)
{
	m_data.resize(n * n);
	float mean = n/2;
	double sum = 0;
	for (int x = 0; x < n; ++x)
	{
		for (int y = 0; y < n; ++y)
		{
			float v = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
					  / (2 * M_PI * sigma * sigma);
			m_data[x * n + y] = v;
			sum += v;
		}
	}

	for (int i = 0; i < m_data.size(); ++i)
	{
		m_data[i] = m_data[i] / sum;
	}
}

void GaussFilter::filterImage(std::vector<float> &image, int rows, int cols) const
{

	int c = m_n / 2;

	for (int x = c; x < (rows - c); ++x)
	{
		for (int y = c; y < (cols - c); ++y)
		{
			double val = 0;
			for (int xx = -c; xx <= c; ++xx)
			{
				for (int yy = -c; yy <= c; ++yy)
				{
					float v1 = image[(x + xx) * rows + (y + yy)];
					float v2 = m_data[(xx + c) * m_n + (yy + c)];
					val += v1 * v2;
				}
				image[x* rows + y] = val;
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
