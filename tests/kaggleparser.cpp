#include <sstream>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdexcept>
#include <limits>

#include "kaggleparser.h"
#include "filter.h"

#pragma warning (disable : 4996)

static const int normal_size = 28 * 28;

static const int rows = 28;
static const int cols = 28;

//FIXME: rewrite this ugly code
namespace
{
int nextDigit( std::string & rvBuf, int & rvIndx)
{
	std::string  temp;
	while(rvIndx < rvBuf.size())
	{
		const char & c = rvBuf[rvIndx];
		if (c >= '0' && c <= '9')
		{
			++rvIndx;
			temp.push_back(c);
		}
		else
		{
			++rvIndx;
			break;
		}
	}
	if (temp.empty()) return -1;

	return std::stoi(temp);
}
}

KaggleParser::KaggleParser(const std::string &file)
	: m_binaryLevel(false)
{
	m_inp.open(file, std::ifstream::in);
	if (!m_inp.is_open())
		throw std::runtime_error(strerror(errno));
	std::getline(m_inp, m_buf);
}

bool KaggleParser::getNextData(std::vector<unsigned char> &rvData, int *rvDigit)
{
	m_buf.clear();
	std::getline(m_inp, m_buf);

	if (m_buf.empty())
		return false;

	int i = 0;
	int di = 0;
	int digit;
	if (rvDigit)
	{
		digit = nextDigit(m_buf, i);
		if (digit == -1) return false;
		*rvDigit = digit;
	}

	rvData.resize(normal_size);

	while(i < m_buf.size())
	{
		digit = nextDigit(m_buf, i);
		if (digit != -1)
		{
			rvData[di++] = digit;
		}
		else
			return false;
	}
	return true;
}

bool KaggleParser::getNextDataNormalized(std::vector<float> &rvData,
		int *rvDigit)
{
	bool r = getNextData(m_dataBuf, rvDigit);
	rvData.resize(m_dataBuf.size());
	for (size_t i = 0; i < rvData.size(); ++i)
	{
		rvData[i] = float(m_dataBuf[i]) / (float)std::numeric_limits<unsigned char>::max();
	}
	if (m_gauss)
	{
		m_gauss->filterImage(rvData, rows, cols);
	}
	return r;
}

void KaggleParser::setGaussFilter(int n, float sigma)
{
	m_gauss = std::make_shared<GaussFilter>(n, sigma);
}

KaggleBinary::KaggleBinary(const std::string &filePath)
{
	m_inp.open(filePath, std::ifstream::in | std::ifstream::binary);
	if (!m_inp.is_open())
		throw std::runtime_error(strerror(errno));
}

void KaggleBinary::convertToBinary(KaggleParser & parser, const std::string &binFile, bool trainData)
{
	int digit;
	std::vector<float> data;
	std::ofstream fs;
	fs.open(binFile, std::ofstream::out | std::ifstream::binary);
	if (!fs.is_open())
		throw std::runtime_error(strerror(errno));

	int i = 0;
	while (parser.getNextDataNormalized(data, trainData ? &digit : 0))
	{
		if (trainData)
			fs.write((const char *)&digit, sizeof(digit));
		std::size_t t = data.size();
		fs.write((const char *)&t, sizeof(t));
		fs.write((const char *)data.data(), data.size() * sizeof(float));
		++i;
		if ((i % 500) == 0)
		{
			std::cout << "Converting line " << i << "\n";
			std::cout.flush();
		}
	}
}

bool KaggleBinary::getNextData(std::vector<float> &rvData, int *rvDigit)
{
	if (m_inp.eof() || !m_inp.good())
		return false;
	if (rvDigit)
		m_inp.read((char *)rvDigit, sizeof(int));

	std::size_t count;
	m_inp.read((char *)&count, sizeof(count));

	if (count != normal_size)
		return false;

	rvData.resize(count);
	m_inp.read((char *)rvData.data(), count * sizeof(float));
	return true;
}
