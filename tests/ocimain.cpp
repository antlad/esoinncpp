#include <iostream>
#include "ocidataset.h"

int main(int argc, char *argv[])
{
	try
	{
		OCIDataSet ds("/home/antlad/111/optdigits.tra");

		std::vector<float> data;
		int digit;

		int count = 0;
		while(ds.getNextDataNormalized(data, digit))
		{
			std::cout << digit << "\n";
			++count;
		}
		std::cout << "count = " <<  count << "\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	return 0;
}
