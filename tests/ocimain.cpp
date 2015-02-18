#include <iostream>
#include <algorithm>
#include <random>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <esoinn.h>

#include "ocidataset.h"

namespace bf = boost::filesystem;

float doTest(bf::path & dataPath)
{
	ESOINN esoinn(8 * 8, 0.001f, 1.0f, 50, 200);
	bf::path savePath = dataPath / "osi_essoinn.dat";
	if (bf::exists(savePath))
	{
		esoinn.loadFromPath(savePath.string());
	}

	bf::path ociTrainPath = dataPath / "optdigits.tra";
	if (!bf::exists(ociTrainPath))
		std::runtime_error("oci train does not exist");

	OCIDataSet ds(ociTrainPath.string());


	struct Data
	{
		std::vector<float> data;
		int digit;
	};

	std::map<int, Data> trainDataMap;
	std::vector<int> trainQueue;


	int pos = 0;
	while(ds.getNextDataNormalized(trainDataMap[pos].data, trainDataMap[pos].digit))
	{
		trainQueue.push_back(pos++);
	}
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(trainQueue.begin(), trainQueue.end(), g);

	for (int p : trainQueue)
	{
		esoinn.learnNextInput(trainDataMap[p].data, trainDataMap[p].digit);
	}

	esoinn.classificate();

	bf::path ociTestPath = dataPath / "optdigits.tes";
	if (!bf::exists(ociTestPath))
		std::runtime_error("oci test does not exist");

	OCIDataSet ds_test(ociTestPath.string());

	int test_count = 0;
	int success_count = 0;
	std::vector<float> data;
	int digit;
	while(ds_test.getNextDataNormalized(data, digit))
	{
		int32_t out_digit = esoinn.calcInput(data, true);
		if (out_digit == digit)
			++success_count;
		++test_count;
	}

	std::cout << "sub class count " <<  esoinn.subClassesCount() << "\n";

	float success = double(success_count) / double(test_count) * 100;

	return success;
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Invalid args. Set first args as oci dataset data path\n";
			return 1;
		}
		std::string dataPathString(argv[1]);
		if (!bf::exists(dataPathString))
			std::runtime_error((boost::format("Path %s doesn't exists") % dataPathString).str());

		bf::path dataPath(dataPathString);
		double sum = 0;
		for (int i = 0; i < 100; ++i)
		{
			float success = doTest(dataPath);
			std::cout << "iteration " << i << " success " << success << "\n";
			std::cout.flush();
			sum += success;
		}
		std::cout << "total success avarage " << sum / 100 << "\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}
