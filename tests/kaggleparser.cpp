#include <sstream>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

#include "kaggleparser.h"

#pragma warning (disable : 4996)

static const int normal_size = 28 * 28;

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

KaggleBinary::KaggleBinary(const std::string &filePath)
{
	m_inp.open(filePath, std::ifstream::in | std::ifstream::binary);
	if (!m_inp.is_open())
		throw std::runtime_error(strerror(errno));
}

void KaggleBinary::convertToBinary(const std::string &file, const std::string &binFile, bool trainData)
{
	KaggleParser parser(file);

	int digit;
	std::vector<unsigned char> data;
	std::ofstream fs;
	fs.open(binFile, std::ofstream::out | std::ifstream::binary);
	if (!fs.is_open())
		throw std::runtime_error(strerror(errno));

	int i = 0;
	while (parser.getNextData(data, trainData ? &digit : 0))
	{
		if (trainData)
			fs.write((const char *)&digit, sizeof(digit));
		std::size_t t = data.size();
		fs.write((const char *)&t, sizeof(t));
		fs.write((const char *)data.data(), data.size());
		++i;
		if ((i % 500) == 0)
		{
			std::cout << "Converting line " << i << "\n";
			std::cout.flush();
		}
	}
}

bool KaggleBinary::getNextData(std::vector<unsigned char> &rvData, int *rvDigit)
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
	m_inp.read((char *)rvData.data(), count);
	return true;
}
