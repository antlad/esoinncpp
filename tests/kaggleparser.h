#ifndef KAGGLEPARSER_H
#define KAGGLEPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>

class GaussFilter;
/*! Raw kagggle diggit cvs parser
 */
class KaggleParser
{
public:

	/*! Constructor
	 * \param file Absolute path to file
	 */
	KaggleParser(const std::string& file);

	/*! Return new data line from cvs file
     * \param rvData[out] Vector of input data
     * \param rvDigit[out] Pointer to return digit, if null this is prediction
	 * \return False if end reached
	 */
	bool getNextData(std::vector<unsigned char> &rvData, int *rvDigit);

    /*! Return normalized vector
     * \param rvData[out] Vector of input data
     * \param rvDigit[out]  Pointer to return digit, if null this is prediction
     * \return False if end reached
     */
    bool getNextDataNormalized(std::vector<float> &rvData, int *rvDigit);

    /*! Create gauss filter
     * \param n  filter size
     * \param sigma Gauss sigma val
     */
    void setGaussFilter(int n, float sigma);

private:
	std::ifstream m_inp;///< Input stream
	std::string m_buf;	///< Buffer
    std::vector<unsigned char> m_dataBuf; ///< values buffer
    std::shared_ptr<GaussFilter> m_gauss; ///< Gauss filter
    bool m_binaryLevel;
};

/*! Converted binary kaggle diggit parser
 */
class KaggleBinary
{
public:

	/*! Constructor
	 * \param filePath
	 */
	KaggleBinary(const std::string& filePath);

	/*! Convert cvs to binary format
	 * \param file Absolute path to cvs file
	 * \param binFile Absolute path to binary file
	 * \param trainData Falg of train data(inculding digit)
	 */
    static void convertToBinary(KaggleParser &parser, const std::string& binFile, bool trainData);

	/*! Return new data line from binary file
	 * \param rvData Vector of input data
	 * \param rvDigit Pointer to return digit, if null this is prediction
	 * \return False if end reached
	 */
    bool getNextData(std::vector<float> &rvData, int *rvDigit);
private:
	std::ifstream m_inp; ///< Input stream
};


#endif // KAGGLEPARSER_H
