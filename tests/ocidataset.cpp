#include "ocidataset.h"
#include <errno.h>
#include <string.h>
#include <stdexcept>

#include <iostream>

OCIDataSet::OCIDataSet(const std::string& file)
{
	m_inp.open(file, std::ifstream::in);
	if (!m_inp.is_open())
		throw std::runtime_error(strerror(errno));
}

const int inputSize = 64;

bool OCIDataSet::getNextDataNormalized(std::vector<float> &rvData, int &rvDigit)
{
	rvData.resize(inputSize);
	std::getline(m_inp, m_buf);

	if (m_buf.empty())
		return false;

	int dataIndex = 0;
	int digitIndex = 0;
	char digit[2] = {'0', '0'};
	for (int i = 0; i < m_buf.size(); ++i)
	{
		if (m_buf[i] == ',')
		{
			if (digitIndex == 0 || digitIndex > 2)
				throw std::runtime_error("invalid data");

			if (digitIndex == 1)
				rvData[dataIndex++] = float((digit[0] - 48))/16 ;
			else
				rvData[dataIndex++] = float((digit[0] - 48) * 10 + (digit[1] - 48))/16;

			digit[0] = '0';
			digit[1] = '0';
			digitIndex = 0;
		}
		else
			digit[digitIndex++] = m_buf[i];
	}

	if (digitIndex == 1)
		rvDigit = (digit[0] - 48) ;
	else
		rvDigit = (digit[0] - 48) * 10 + (digit[1] - 48);
	return true;
}

OCIDataSet::~OCIDataSet()
{

}

