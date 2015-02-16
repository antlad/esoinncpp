#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#include <cmath>
#include <string>
#include <assert.h>
#include <fstream>

#include "../include/esoinn.h"
#include "esoinn_node.h"
#include "utils.h"

#ifdef BUILD_WITH_PNG_EXPORT_SUPPORT
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
namespace bf = boost::filesystem;
#endif

/*! Esoinn private class
 */
class ESOINN::Private
{
public:

	/*! Constructor
	 */
	Private(size_t inputSize,
			float c1, float c2,
			int maxAge, int iterationThreshold)
		: m_inputSize(inputSize)
		, m_c1(c1)
		, m_c2(c2)
		, m_maxAge(maxAge)
		, m_iterationThreshold(iterationThreshold)
		, m_subClassCounter(0)
		, m_iteration(0)
		, m_id(0)
	{
	}

	// Returning count of Esoinn nodes
	size_t size() const;

	// Return sublasses count
	size_t subClassCount() const;

	// Loading Esoinn map from file
	void loadFromPath(const std::string& abthPath);

	// Saving Esoinn map to file
	void saveToPath(const std::string& abthPath) const;

	// Teach Esoinn new data
	void learnNextInput(const std::vector<float>& x,
						int32_t realLabel);

	// Simply calc vector and return prediction of label
	int32_t calcInput(const std::vector<float>& x) const;

	// Simply calc vector and return prediction of label and try to learn
	int32_t calcInputAndLearn(const std::vector<float>& x);

	/*! Start classificate procedure
	 */
	void classificate();

	/*! Save main apexes to folder
	 */
	void saveApexesToFolder(const std::string &folderPath, int rows, int cols) const;

	NodeInfo nodeInfo(std::size_t i) const;

	bool wasClassificationStep() const;

	std::map<std::size_t, std::vector<std::size_t> > getLinks() const;

	int validateLinks() const;
private:

	/*! Part of learning process
	 * \param x Input vector
	 * \param realLabel Real label
	 */
	void modeToData(const std::vector<float>& x, int32_t realLabel, ESOINNNode *w1 = 0, ESOINNNode *w2 = 0);

	/*! Calculating similarity threshol
	 * \param node Node to calculate
	 * \return Returning similarity threshold
	 */
	float similarityThreshold(const ESOINNNode* node) const;

	/*! Processing new input vector
	 * \param x Input vector
	 */
	void procInput(const std::vector<float>& x) const;

	/*! Make sub classes process
	 */
	void makeSubClasses();

	/*! Find max density of sub class
	 * \param subClass
	 * \return Max Density
	 */
	double maxSubClassDensity(int64_t subClass) const;

	/*! Check if we need connect two winners
	 * \param w1 Main winner
	 * \param w2 Second winner
	 * \return Need connect
	 */
	bool needConnectionCheck(ESOINNNode* w1, ESOINNNode* w2) const;

	/*! Calculate and return sum density of sub class
	 * \param subClass Sub class
	 * \param rvCountCount of neurons with this sub class
	 * \return Sum of density
	 */
	double subClassDensitySum(int64_t subClass, size_t* rvCount) const;

	/*! Calculate and return mean subclass density
	 * \param subClass Sub class
	 * \return Mean Density
	 */
	double subClassDensityMean(int64_t subClass) const;

	/*! Check if we need merge for sublasses
	 * \param a Neuron with subclass A
	 * \param b Neuron with subclass B
	 * \return Need merge
	 */
	bool needMergeSubClassCheck(ESOINNNode* a, ESOINNNode* b) const;

	/*! Cleaning noise step
	 */
	void clearNoiseProc();

	/*! Getting alpha cofficient
	 * \param mean Mean
	 * \param maxDensity Max density
	 * \return Alpha
	 */
	inline double getAlpha(double mean, double maxDensity) const;

	/*! Clearing old ages
	 */
	void removeOldAges();

	/*! Checking similarity threshold for two winners
	 * \param w1 Main winner
	 * \param w2 Second winner
	 * \return If not then remember this data
	 */
	bool isWithinThreshold(ESOINNNode* w1, ESOINNNode* w2);

	/*! Returning all neurons const reference
	 * \return All neurons const reference
	 */
	const std::vector<ESOINNNodePtr> &neurons() const;

	/*! Find winners step
	 * \param rvW1[out] Returning main winner
	 * \param rvW2[out] Returning second winner
	 */
	void findWinners(ESOINNNode*& rvW1, ESOINNNode*& rvW2) const;

	std::vector<ESOINNNodePtr> m_neurons;	///< neurons
	bool m_thisWasClassificationStep;		///<
	size_t m_inputSize;						///< Input vector size
	uint64_t m_iteration;					///< Current data interation
	uint64_t m_subClassCounter;				///< Sub class number counter
	uint64_t m_id;							///< Sub class id
	int m_iterationThreshold;				///< Threshold of creating sub classes and clearing subclasses
	float m_c1;								///< C1 constant
	float m_c2;								///< C2 constant
	int m_maxAge;							///< Max age of node
};

ESOINN::ESOINN(size_t inputSize, float c1, float c2, int maxAge, int iterationThreshold)
	: d(new Private(inputSize, c1, c2, maxAge, iterationThreshold))
{
	std::srand((unsigned int)std::time(0));
}

int32_t ESOINN::calcInput(const std::vector<float> &x, bool learnData) const
{
	if (!learnData)
		return d->calcInput(x);
	else
		return d->calcInputAndLearn(x);
}

void ESOINN::learnNextInput(const std::vector<float> &x, int32_t realLabel) const
{
	d->learnNextInput(x, realLabel);
}

void ESOINN::loadFromPath(const std::string &abthPath) const
{
	d->loadFromPath(abthPath);
}

void ESOINN::saveToPath(const std::string &abthPath) const
{
	d->saveToPath(abthPath);
}

void ESOINN::classificate() const
{
	d->classificate();
}

std::size_t ESOINN::size() const
{
	return d->size();
}

std::size_t ESOINN::subClassesCount() const
{
	return d->subClassCount();
}

NodeInfo ESOINN::nodeInfo(std::size_t i) const
{
	return d->nodeInfo(i);
}

std::map<std::size_t, std::vector<std::size_t> > ESOINN::getLinks() const
{
	return d->getLinks();
}

bool ESOINN::wasClassificationStep() const
{
	return d->wasClassificationStep();
}

int ESOINN::validateLinks() const
{
	return d->validateLinks();
}

#ifdef BUILD_WITH_PNG_EXPORT_SUPPORT
void ESOINN::saveApexesToFolder(const std::string &folderPath, int rows, int cols) const
{
	d->saveApexesToFolder(folderPath, rows, cols);
}
#endif

void ESOINN::Private::classificate()
{
	makeSubClasses();
	clearNoiseProc();
}

#ifdef BUILD_WITH_PNG_EXPORT_SUPPORT
struct ApexDesc
{
	ESOINNNode* apex;
	std::list<ESOINNNode*> area;
};

void ESOINN::Private::saveApexesToFolder(const std::string &folderPath, int rows, int cols) const
{
	if (folderPath.empty())
		throw std::invalid_argument("folderPath is empty!");
	if (!bf::exists(folderPath))
		throw std::invalid_argument(str(boost::format("folder %1 doesn't exists!") % folderPath));

	std::map<int32_t, ApexDesc> subs;

	for (const ESOINNNodePtr & n : m_neurons)
	{
		auto it = subs.find(n->subClass());
		if (it == subs.end())
		{
			subs[n->subClass()].apex = n.get();
			continue;
		}
		else
		{
			ApexDesc & aDesc = (*it).second;
			if (aDesc.apex->density() < n->density())
			{
				ESOINNNode* a = aDesc.apex;
				aDesc.apex = n.get();
				aDesc.area.push_back(a);
			}
			else
			{
				aDesc.area.push_back(n.get());
			}
		}
	}

	bf::path dataFolder(folderPath);
	dataFolder = dataFolder / bf::path("png");
	if (bf::exists(dataFolder))
	{
		if (!bf::remove_all(dataFolder))
			throw std::runtime_error("Can't remove png directory!");
	}
	if (!bf::create_directory(dataFolder))
		throw std::runtime_error("Can't create dirrectory for png!");

	for (auto it = subs.begin(); it != subs.end(); ++it)
	{
		ApexDesc & aDesc = (*it).second;

		auto saveToPngFunc = [&](const ESOINNNode* n, const bf::path & folder, int i)
		{
			std::string f = (boost::format("s_%d_d_%d_i_%d.png")
							 % n->subClass()
							 % n->realLabel()
							 % i).str();
			n->saveToPng((folder / bf::path(f)).string(), rows, cols);
		};

		saveToPngFunc(aDesc.apex, dataFolder, 0);

		if (!aDesc.area.empty())
		{
			bf::path subFolder((boost::format("s_%d_d_%d")
								% aDesc.apex->subClass()
								% aDesc.apex->realLabel()).str());
			if (!bf::create_directory(dataFolder / subFolder))
				throw std::runtime_error("Can't create dirrectory for sub class images!");
			int i = 0;
			for (const ESOINNNode* n : aDesc.area)
			{
				saveToPngFunc(n, dataFolder / subFolder, ++i);
			}
		}
	}
}

#endif

NodeInfo ESOINN::Private::nodeInfo(std::size_t i) const
{
	if (i < 0 || i > m_neurons.size())
		throw std::runtime_error("Invalid node number");

	NodeInfo info;
	const ESOINNNodePtr & n = m_neurons[i];
	info.weights = n->weights();
	info.subClass = n->subClass();
	info.density = n->density();
	info.winCount = n->winCount();
	info.distance = n->distance();
	info.realLabel = n->realLabel();
	return info;
}

bool ESOINN::Private::wasClassificationStep() const
{
	return m_thisWasClassificationStep;
}

std::map<std::size_t, std::vector<std::size_t> > ESOINN::Private::getLinks() const
{
	std::map<uint64_t, std::size_t> idToPos;
	std::map<std::size_t, std::vector<std::size_t> > result;
	std::size_t pos = 0;
	for (const ESOINNNodePtr & n : m_neurons)
	{
		idToPos[n->nodeId()] = pos++;
	}
	for (const ESOINNNodePtr & n : m_neurons)
	{
		std::vector<uint64_t> ids = n->idLinks();
		std::size_t pos = idToPos[n->nodeId()];
		result[pos].resize(ids.size());
		for (std::size_t i = 0; i < ids.size(); ++i)
		{
			result[pos][i] = idToPos[ids[i]];
		}
	}
	return result;
}

int ESOINN::Private::validateLinks() const
{
	int fails = 0;
	for (const ESOINNNodePtr & n : m_neurons)
	{
		fails += n->validateLinks();
	}
	return fails;
}


float ESOINN::Private::similarityThreshold(const ESOINNNode *node) const
{
	if (node->linksCount())
		return node->maxDistanceToNeibs();

	size_t size = m_neurons.size();
	//std::vector<float> results;
	//results.resize(size);

	//#pragma omp parallel for
	float T = std::numeric_limits<float>::max();
	for (int i = 0; i < size; ++i)
	{
		ESOINNNode* n = m_neurons[i].get();
		if (n != node)
		{
			float dist = node->distanceTo(n);
			if (dist < T)
				T = dist;
		}
//		if (n != node)
//		{
//			results[i] = node->distanceTo(n);
//		}
//		else
//			results[i] = std::numeric_limits<float>::max();
	}


//	for (const float& dist : results)
//	{
//		if (dist < T)
//			T = dist;
//	}

	return T;
}

void ESOINN::Private::procInput(const std::vector<float> &x) const
{
	//Proc new input vector
	std::size_t size = m_neurons.size();
	const std::vector<ESOINNNodePtr> & n = m_neurons;
	//#pragma omp parallel for
	for (int i = 0 ; i < size; ++i)
	{
		n[i]->procInput(x);
	}
}

void ESOINN::Private::makeSubClasses()
{
	for (ESOINNNodePtr& n : m_neurons)
	{
		n->setSubClass(-1);
	}
	int32_t subClassCount = 0;
	std::sort(m_neurons.begin(), m_neurons.end(), [](const ESOINNNodePtr & a, const ESOINNNodePtr & b) -> bool
	{
		return a->density() > b->density();
	});

	for (ESOINNNodePtr& n : m_neurons)
	{
		if (n->subClass() == -1)
		{
			n->setSubClass(++subClassCount);
			n->setNeibsSubClass(subClassCount, n.get());
		}
	}

	for (ESOINNNodePtr& nPtr : m_neurons)
	{
		const std::vector<ESOINNNode*>& overlaped = nPtr->overlapedList();
		if (!overlaped.empty())
		{
			for (ESOINNNode* node : overlaped)
			{
				if (node->subClass() == nPtr->subClass())
					continue;

				if (needMergeSubClassCheck(nPtr.get(), node))
				{
					node->mergeWithNewSubClass(nPtr->subClass());
				}
			}
		}
	}

	for (ESOINNNodePtr& n : m_neurons)
		n->splitNoise();
}

double ESOINN::Private::maxSubClassDensity(int64_t subClass) const
{
	double maxDensity = std::numeric_limits<float>::min();
	for (const ESOINNNodePtr& n : m_neurons)
	{
		if (n->subClass() == subClass)
		{
			if (maxDensity < n->density())
				maxDensity = n->density();
		}
	}
	return maxDensity;
}

bool ESOINN::Private::needConnectionCheck(ESOINNNode *w1, ESOINNNode *w2) const
{
//	if (w1->realLabel() != UNKNOW_LABEL && w2->realLabel() != UNKNOW_LABEL)
//		return w1->realLabel() == w2->realLabel();

	if (w1->subClass() == -1 || w2->subClass() == -1) return true;
	else if (w1->subClass() == w2->subClass()) return true;
	else
	{
		assert(w1->subClass() != w2->subClass());
		return needMergeSubClassCheck(w1, w2);
	}
}

double ESOINN::Private::subClassDensitySum(int64_t subClass, size_t *rvCount) const
{
	double densitySum = 0;
	size_t count = 0;
	for (const ESOINNNodePtr& n : m_neurons)
	{
		if (n->subClass() == subClass)
		{
			densitySum += n->density();
			++count;
		}
	}
	if (rvCount)
		*rvCount = count;
	return densitySum;
}

double ESOINN::Private::subClassDensityMean(int64_t subClass) const
{
	size_t count = 0;
	double meanDensity = subClassDensitySum(subClass, &count);
	if (count)
		return meanDensity /= count;
	else return 0;
}

bool ESOINN::Private::needMergeSubClassCheck(ESOINNNode *a, ESOINNNode *b) const
{
//	if (a->realLabel() != UNKNOW_LABEL && b->realLabel() != UNKNOW_LABEL)
//		return a->realLabel() == b->realLabel();

	int64_t A = a->subClass();
	double winDensityMin = std::min(a->density(), b->density());
	double meanA = subClassDensityMean(A);
	double maxA = maxSubClassDensity(A);
	double alphaA = getAlpha(meanA, maxA);

	int64_t B = b->subClass();
	double meanB = subClassDensityMean(B);
	double maxB = maxSubClassDensity(B);
	double alphaB = getAlpha(meanB, maxB);

	if (winDensityMin > maxA * alphaA && winDensityMin > maxB * alphaB)
		return true;

	return false;
}

void ESOINN::Private::clearNoiseProc()
{
	//check to delete noise
	std::set<ESOINNNode*> todelete;
	auto clean = [&]() -> int
	{
		int count = 0;
		for (ESOINNNodePtr& n : m_neurons)
		{
			double mean = subClassDensityMean(n->subClass());
			if (n->linksCount() == 2 &&
					n->density() < mean * m_c1)
			{
				n->destroy();
				todelete.insert(n.get());
				++count;
			}
			else if (n->linksCount() == 1 &&
					 n->density() < mean * m_c2)
			{
				n->destroy();
				todelete.insert(n.get());
				++count;
			}
			else if (n->linksCount() == 0)
			{
				n->destroy();///realy need???
				todelete.insert(n.get());
				++count;
			}
		}
		return count;
	};

	while(clean())
	{
		auto rmn_it = std::remove_if(m_neurons.begin(), m_neurons.end(), [&](ESOINNNodePtr & n)
		{
			return todelete.find(n.get()) != todelete.end();
		});

		m_neurons.erase(rmn_it, m_neurons.end());
	}
}

double ESOINN::Private::getAlpha(double mean, double maxDensity) const
{
	if ((2 * mean) >= maxDensity)
		return 0;
	else if ((3 * mean) >= maxDensity &&
			 maxDensity > (2 * mean))
		return 0.5;
	else
		return 1;
}

void ESOINN::Private::removeOldAges()
{
	for (ESOINNNodePtr& n : m_neurons)
	{
		n->clearOldLinks(m_maxAge);
	}
}

bool ESOINN::Private::isWithinThreshold(ESOINNNode *w1, ESOINNNode *w2)
{
	if (w1->distance() > similarityThreshold(w1))
	{
		return false;
	}
	if (w2->distance() > similarityThreshold(w2))
	{
		return false;
	}
	return true;
}

const std::vector<ESOINNNodePtr> &ESOINN::Private::neurons() const
{
	return m_neurons;
}

int32_t ESOINN::Private::calcInput(const std::vector<float> &x) const
{
	if (m_neurons.size() < 2)
		std::runtime_error("ESOINN is empty!");
	procInput(x);

	ESOINNNode* n1 = 0;
	ESOINNNode* n2 = 0;

	findWinners(n1, n2);
	return n1->realLabel();
}

int32_t ESOINN::Private::calcInputAndLearn(const std::vector<float> &x)
{
	++m_iteration;
	if (m_neurons.size() < 2)
	{
		ESOINNNodePtr n(new ESOINNNode(x, ++m_id));
		m_neurons.push_back(n);
		return UNKNOW_LABEL;
	}

	procInput(x);

	ESOINNNode* n1 = 0;
	ESOINNNode* n2 = 0;

	findWinners(n1, n2);
	int r = n1->realLabel();
	modeToData(x, r, n1, n2);
	return r;
}

void ESOINN::Private::modeToData(const std::vector<float> &x, int32_t realLabel, ESOINNNode* w1, ESOINNNode* w2)
{
	//ESOINNNode* w1 = 0;
	//ESOINNNode* w2 = 0;

	m_thisWasClassificationStep = false;

	if (w1 == 0 && w2 == 0)
	{
		findWinners(w1, w2);
	}

	if (!isWithinThreshold(w1, w2))
	{
		ESOINNNodePtr n(new ESOINNNode(x, ++m_id));
		n->setRealLabel(realLabel);
		m_neurons.push_back(n);
		return;
	}

	//increment ages
	w1->incrementLinksAge();

	//judge if it is necessary to build a connection
	if (needConnectionCheck(w1, w2))
	{
		w1->addLink(w2);
		w2->addLink(w1);
	}
	else
	{
		w1->removeLink(w2);
		w2->removeLink(w1);
	}

	//Increment signals count
	//Update density
	//double d = 0; ;
//	for (const ESOINNNodePtr& n : m_neurons)
//	{
//		d += w1->distanceTo(n.get());
//	}
//	d /= m_neurons.size();

	w1->incrementWinCount();
	w1->updateDensity(w1->meanDistanceToNeibs());

	//Adapt weights
	float E1 = 1 / float(w1->winCount());
	float E2 = 1 / (float(100) * float(w1->winCount()));
	w1->adaptWeight(E1, E2, x);

	//Remove old edges
	removeOldAges();

	if (m_iteration % m_iterationThreshold == 0)
	{
		makeSubClasses();
		clearNoiseProc();
		m_thisWasClassificationStep = true;
	}
}

void ESOINN::Private::learnNextInput(const std::vector<float> &x, int32_t realLabel)
{
	++m_iteration;
	if (m_neurons.size() < 2)
	{
		ESOINNNodePtr n(new ESOINNNode(x, ++m_id));
		n->setRealLabel(realLabel);
		m_neurons.push_back(n);
		return;
	}

	procInput(x);
	modeToData(x, realLabel);
}

void ESOINN::Private::loadFromPath(const std::string &abthPath)
{
	if (abthPath.empty())
		throw std::runtime_error("Empty file path while load");

	std::ifstream fs;

	fs.open(abthPath, std::ifstream::binary | std::ifstream::in);
	if (!fs.is_open())
		throw std::runtime_error(std::string("Can't open for read: ").append(abthPath));

	ut::read(fs, m_inputSize);
	ut::read(fs, m_iterationThreshold);
	ut::read(fs, m_c1);
	ut::read(fs, m_c2);
	ut::read(fs, m_maxAge);
	ut::read(fs, m_iteration);
	ut::read(fs, m_subClassCounter);
	ut::read(fs, m_id);

	std::size_t size;
	ut::read(fs, size);

	m_neurons.clear();
	std::map<uint64_t, ESOINNNode*> neurons;
	for (std::size_t i = 0; i < size; ++i)
	{
		ESOINNNodePtr n = std::make_shared<ESOINNNode>();
		n->loadFromStream(fs);
		m_neurons.push_back(n);
		neurons[n->nodeId()] = n.get();
	}

	for (ESOINNNodePtr& n: m_neurons)
	{
		n->loadLinksFromStream(fs, neurons);
	}
}

void ESOINN::Private::saveToPath(const std::string &abthPath) const
{
	if (abthPath.empty())
		throw std::runtime_error("Empty file path while save");

	std::ofstream fs;
	fs.open(abthPath, std::ofstream::binary | std::ofstream::trunc | std::ofstream::out);
	if (!fs.is_open())
		throw std::runtime_error(std::string("Can't open for write: ").append(abthPath));

	ut::write(fs, m_inputSize);
	ut::write(fs, m_iterationThreshold);
	ut::write(fs, m_c1);
	ut::write(fs, m_c2);
	ut::write(fs, m_maxAge);
	ut::write(fs, m_iteration);
	ut::write(fs, m_subClassCounter);
	ut::write(fs, m_id);

	ut::write(fs, m_neurons.size());
	for (const ESOINNNodePtr& n : m_neurons)
	{
		n->saveToStream(fs);
	}

	for (const ESOINNNodePtr& n : m_neurons)
	{
		n->saveLinksToStream(fs);
	}
}

std::size_t ESOINN::Private::size() const
{
	return m_neurons.size();
}

std::size_t ESOINN::Private::subClassCount() const
{
	std::set<int32_t> subClasses;
	for (const ESOINNNodePtr& n : m_neurons)
	{
		subClasses.insert(n->subClass());
	}
	return subClasses.size();
}

void ESOINN::Private::findWinners(ESOINNNode *&rvW1, ESOINNNode *&rvW2) const
{
	float bestDist1 = std::numeric_limits<float>::max();
	float bestDist2 = std::numeric_limits<float>::max();
	rvW1 = 0;
	rvW2 = 0;

	//Find winners
	for (const ESOINNNodePtr& n : m_neurons)
	{
		assert(n.get());

		float dist = n->distance();
		assert(!std::isnan(dist));

		if (dist < bestDist1)
		{
			bestDist2 = bestDist1;
			rvW2 = rvW1;
			bestDist1 = dist;
			rvW1 = n.get();
		}
		else if (dist < bestDist2)
		{
			bestDist2 = dist;
			rvW2 = n.get();
		}
	}
	assert(rvW2);
	assert(rvW1);
}

