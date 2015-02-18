#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <string>

class GaussFilter
{
public:
    GaussFilter(int n, float sigma);

    void filterImage(std::vector<float> & image, int rows, int cols) const;

	static void savePNG(const std::string & str, const std::vector<float> & weights, int rows, int cols);
private:

    int m_n;
	std::vector<float> m_data;
};

#endif // FILTER_H
