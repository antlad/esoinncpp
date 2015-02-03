#ifndef OCIDATASET_H
#define OCIDATASET_H

#include <string>
#include <fstream>
#include <vector>

class OCIDataSet
{
public:
	OCIDataSet(const std::string& file);

	bool getNextDataNormalized(std::vector<float> &rvData, int &rvDigit);

	~OCIDataSet();
private:
	std::ifstream m_inp;///< Input stream
	std::string m_buf;	///< Buffer
};

#endif // OCIDATASET_H
