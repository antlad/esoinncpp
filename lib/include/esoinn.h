#ifndef ESOINN_H
#define ESOINN_H

#include <vector>
#include <memory>

#include "esoinn_export.h"

/** Main ESOINN class
 */
class ESOINN_EXPORT ESOINN
{
public:

	/*! Constructor
	 * \param inputSize Input vector size
	 * \param c1 C1 constant
	 * \param c2 C2 constant
	 * \param maxAge Max node age
	 * \param iterationThreshold  Clean threshold
	 */
	ESOINN(size_t inputSize,
		   float c1, float c2,
		   int maxAge, int iterationThreshold);

	/*! Simply calc vector and return prediction of label
	 * \param x Input vector
	 * \param learnData Try to learn data
	 * \return Prediction of label
	 */
	int calcInput(const std::vector<float>& x, bool learnData = false) const;

	/*! Teach Esoinn new data
	 * \param x Input vector
	 * \param realLabel Real label
	 */
	void learnNextInput(const std::vector<float>& x,
						int realLabel) const;

	/*! Loading Esoinn map from file
	 * \param abthPath Absolute file path
	 */
	void loadFromPath(const std::string& abthPath) const;

	/*! Saving Esoinn map to file
	 * \param abthPath Absolute file path
	 */
	void saveToPath(const std::string& abthPath) const;

	/*! Start classificate procedure
	 * Makeing subclasses and clean
	 */
	void classificate() const;

	/*! Returning count of Esoinn nodes
	 * \return Count of of Esoinn nodes
	 */
	size_t size() const;

    /*! Returning count of subclasses
     * \return Count of subclasses
     */
    size_t subClassesCount() const;

private:
	class Private;
	std::shared_ptr<Private> d; ///< Private part of class
};

#endif // ESOINN_H
