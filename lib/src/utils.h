#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <stdexcept>

namespace ut
{

template<class T>
void write(std::ostream & os, const T & val)
{
	os.write((const char *)&val,sizeof(val));
	if (os.bad())
		throw std::runtime_error("error while writing file");
}

template<class T>
void read(std::istream & is, T & rvVal)
{
	is.read((char*)&rvVal, sizeof(rvVal));
	if (is.bad())
		throw std::runtime_error("error while reading file");
}

}

#endif // UTILS_H
