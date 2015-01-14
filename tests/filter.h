#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <string>

class GaussFilter
{
public:
	GaussFilter(uint8_t n, float sigma);

	void print();

	void filterImage(std::vector<float> & image, int rows, int cols);

	static void savePNG(const std::string & str, const std::vector<float> & weights, int rows, int cols);
private:

	uint8_t m_n;
	std::vector<float> m_data;
};

#endif // FILTER_H
