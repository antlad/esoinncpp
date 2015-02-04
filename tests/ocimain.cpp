#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <esoinn.h>

#include "ocidataset.h"

namespace bf = boost::filesystem;

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

        ESOINN esoinn(8 * 8, 0.001f, 1.0f, 50, 200);
        bf::path savePath = dataPath / "osi_essoinn.dat";
        if (bf::exists(savePath))
        {
            esoinn.loadFromPath(savePath.string());
        }

        bf::path ociTrainPath = dataPath / "optdigits.tra";
        if (!bf::exists(ociTrainPath))
            std::runtime_error("");

        OCIDataSet ds(ociTrainPath.string());

		std::vector<float> data;
		int digit;

		while(ds.getNextDataNormalized(data, digit))
		{
            esoinn.calcInput(data, true);
		}
        esoinn.classificate();

        esoinn.saveApexesToFolder(dataPathString, 8, 8);

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
        return 1;
	}

	return 0;
}
