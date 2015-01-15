#include <iostream>
#include <errno.h>
#include <string.h>
#include <limits>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/system/api_config.hpp>
#include <boost/foreach.hpp>

#include "kaggleparser.h"
#include "esoinn.h"



//#define CHECK_ONLY
//#define CHECK_COUNT 2000
#pragma warning (disable : 4996)


namespace bf = boost::filesystem;

/*! Train Esoinn on test data
 * \param esoinn Esoinn map
 * \param dataFolder Path to data folder
 */
void train(ESOINN & esoinn, const std::string & dataFolder)
{
	uint64_t t = 0;
	int digit;

    std::vector<float> data;
	bf::path p(dataFolder);
	bf::path f("train.bin");
	bf::path totalPath = p / f;

	if (!boost::filesystem::exists(totalPath))
	{
		std::cout << "start creating train.bin\n" << std::flush;

        KaggleParser parser((p / bf::path("train.csv")).string());
        parser.setGaussFilter(5, 1.0);

        KaggleBinary::convertToBinary(parser, totalPath.string(), true);
		std::cout << "done creating train.bin\n" << std::flush;
	}
	KaggleBinary trainParser(totalPath.string());

    while (trainParser.getNextData(data, &digit))
	{
        esoinn.learnNextInput(data, digit);

		++t;
		if ((t % 200) == 0)
		{
			std::cout << "count = " << t << " size: " << esoinn.size() << "\n";
			std::cout.flush();
		}

#ifdef CHECK_ONLY
		if ((t % CHECK_COUNT) == 0)
			break;
#endif
	}

	esoinn.classificate();
	std::cout << "ESOINN done " << t << "\n";
	std::cout << "esoinn size = " << esoinn.size() << "\n";
	std::cout.flush();
}

/*! CheckEsoinn on test data
 * \param esoinn Esoinn map
 * \param dataFolder Path to data folder
 * \return
 */
void test(ESOINN & esoinn, const std::string & dataFolder)
{
	bf::path p(dataFolder);
	KaggleBinary testParser((p / bf::path("train.bin")).string());

	uint64_t t = 0;
	int digit;
    std::vector<float> data;
	int fail = 0;
    while (testParser.getNextData(data, &digit))
	{
        uint64_t label = esoinn.calcInput(data, false);
		if (label != digit)
			++fail;

		++t;
		if ((t % 200) == 0)
		{
			std::cout << "count = " << t << "\n";
			std::cout.flush();
		}

#ifdef CHECK_ONLY
		if ((t % CHECK_COUNT) == 0)
			break;
#endif
	}
	std::cout << "fails on train data: " << (float(fail) / t) * 100 << "\n";
	std::cout.flush();

}

/*! Make pradiction to out.txt in data folder
 * \param esoinn Esoinn map
 * \param dataFolder Path to data folder
 */
void predict(ESOINN & esoinn, const std::string & dataFolder, bool train)
{
	uint64_t t = 0;
    std::vector<float> data;
	bf::path p(dataFolder);
	bf::path total = p / bf::path("test.bin");
	if (!boost::filesystem::exists(total))
	{
		std::cout << "start creating test.bin\n" << std::flush;
        KaggleParser parser((p / bf::path("test.csv")).string());
        parser.setGaussFilter(5, 1.0);
        KaggleBinary::convertToBinary(parser, total.string(), false);
		std::cout << "done creating test.bin\n" << std::flush;
	}
	KaggleBinary predictParser(total.string());
	std::cout << "start predict step\n";
	std::ofstream fso;

	fso.open((p / bf::path("out.txt")).string(), std::ofstream::out | std::ofstream::trunc);
	fso << "ImageId,Label\n";
	t = 0;
	if (!fso.is_open())
		throw std::runtime_error(strerror(errno));

	while (predictParser.getNextData(data, 0))
	{
        uint64_t label = esoinn.calcInput(data, train);
		fso << std::to_string(++t) << "," << std::to_string(label) << "\n";

		if ((t % 200) == 0)
		{
			std::cout << "prediction count = " << t << "\n";
			std::cout.flush();
		}
#ifdef CHECK_ONLY
		if ((t % CHECK_COUNT) == 0)
			break;
#endif
	}
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Invalid args. Set first args as Kaggle data path\n";
			return 1;
		}
		std::string dataPathString(argv[1]);
		if (!bf::exists(dataPathString))
		{
			std::cerr << "Path " << dataPathString << " doesn't exists \n";
			return 1;
		}
		bf::path dataPath(dataPathString);

        ESOINN esoinn(28 * 28, 0.001f, 0.8f, 50, 100);
		bf::path savePath = dataPath / "kaggle_essoinn.dat";
		if (bf::exists(savePath))
		{
			esoinn.loadFromPath(savePath.string());
		}

		std::chrono::time_point<std::chrono::system_clock> start, end;

		start = std::chrono::system_clock::now();
		train(esoinn, dataPathString);
        std::cout << "train done, size=" << esoinn.size() << " subClasses count= " << esoinn.subClassesCount() << "\n";
        //predict(esoinn, dataPathString, true);
        esoinn.classificate();

        test(esoinn, dataPathString);
        esoinn.saveApexesToFolder(dataPathString, 28, 28);
        return 0;
		predict(esoinn, dataPathString, false);

		end = std::chrono::system_clock::now();



		uint64_t elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
							  (end-start).count();

		std::cout << "calc done in " << elapsed_seconds << "\n";
		esoinn.saveToPath(savePath.string());
		std::cout << "all done\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
	catch (...)
	{
		std::cerr << "unknown exception :(";
	}
	return 0;
}
