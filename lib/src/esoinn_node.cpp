#include <assert.h>
#include <limits>
#include <set>
#include <cmath>
#include <algorithm>

#include <fstream>

#include "utils.h"
#include "esoinn_node.h"

#ifdef BUILD_WITH_PNG_EXPORT_SUPPORT
#include <png++/png.hpp>
#endif

namespace
{

static float vectorDistance(const std::vector<float>& x, const std::vector<float>& y)
{
	assert(x.size() == y.size());
	size_t size = x.size();
	const float* xP = x.data();
	const float* yP = y.data();
	double dist = 0;
	for (size_t i = 0; i < size; ++i, ++xP, ++yP)
	{
		dist += (*xP - *yP) * (*xP - *yP);
	}

	return sqrt(dist);
}

}

void ESOINNNode::adaptWeight(float E1, float E2,
							 const std::vector<float>& x)
{
	assert(x.size() == m_weights.size());
	size_t size = x.size();
	{
		float* wPtr = m_weights.data();
		const float* xPtr = x.data();
		for (size_t i = 0; i < size; ++i, ++wPtr, ++xPtr)
		{
			assert(!std::isinf(*wPtr));
			*wPtr += E1 * (*xPtr - *wPtr);
			assert(!std::isinf(*wPtr));
		}
	}

	for (std::map<ESOINNNode*, uint16_t>::iterator it = m_links.begin();
			it != m_links.end(); ++it)
	{
		ESOINNNode* n = it->first;
		float* wPtr = n->m_weights.data();
		const float* xPtr = x.data();

		for (size_t i = 0; i < size; ++i, ++wPtr, ++xPtr)
		{
			assert(!std::isinf(*wPtr));
			*wPtr += E2 * ((*xPtr) - (*wPtr));
			assert(!std::isinf(*wPtr));
		}
	}
}

float ESOINNNode::distance() const
{
	return m_dist;
}

std::vector<ESOINNNode*> ESOINNNode::overlapedList()
{
	std::vector<ESOINNNode*> result;
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;

		if (node->m_subClass != m_subClass)
			result.push_back(node);
	}
	return result;
}

void ESOINNNode::mergeWithNewSubClass(int32_t newSubClass)
{
	if (newSubClass == m_subClass)
		return;

	int32_t oldSubClass = m_subClass;
	m_subClass = newSubClass;
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;
		if (node->m_subClass != oldSubClass)
			continue;

		node->mergeWithNewSubClass(newSubClass);
	}
}

void ESOINNNode::splitNoise()
{
	for (auto it = m_links.begin(); it != m_links.end();)
	{
		ESOINNNode* node = it->first;
		assert(node);
		assert(node->m_subClass != UNKNOW_LABEL);
		if (node->m_subClass != m_subClass ||
				(node->m_realLabel != m_realLabel))
		{
			node->removeLink(this);
			m_links.erase(it++);
		}
		else
			++it;
	}
}

void ESOINNNode::destroy()
{
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;
		node->removeLink(this);
	}
	m_links.clear();
}

ESOINNNode::ESOINNNode(const std::vector<float>& weights, uint64_t id)
	: m_dist(0)
	, m_s(0)
	, m_density(0)
	, m_subClass(UNKNOW_LABEL)
	, m_realLabel(UNKNOW_LABEL)
	, m_winCount(0)
	, m_id(id)
	, m_weights(weights)
{
}

ESOINNNode::ESOINNNode()
	: m_dist(0)
	, m_s(0)
	, m_density(0)
	, m_subClass(UNKNOW_LABEL)
	, m_realLabel(UNKNOW_LABEL)
	, m_winCount(0)
	, m_id(0)
{
}

void ESOINNNode::incrementLinksAge()
{
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		++(it->second);
		ESOINNNode* n = it->first;
		n->m_links[this] += 1;
	}
}

void ESOINNNode::clearOldLinks(int maxAge)
{
	auto it = m_links.begin();
	while (it != m_links.end())
	{
		if ((*it).second > maxAge)
		{
			it->first->removeLink(this);
			m_links.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

const std::vector<float>& ESOINNNode::weights() const
{
	return m_weights;
}

void ESOINNNode::addLink(ESOINNNode* n)
{
	m_links[n] = 0;
}

void ESOINNNode::removeLink(ESOINNNode* n)
{
	m_links.erase(n);
}

uint32_t ESOINNNode::winCount()
{
	return m_winCount;
}

void ESOINNNode::incrementWinCount()
{
	++m_winCount;
}

void ESOINNNode::procInput(const std::vector<float>& x)
{
	m_dist = vectorDistance(m_weights, x);
}

std::size_t ESOINNNode::linksCount() const
{
	return m_links.size();
}

float ESOINNNode::distanceTo(const ESOINNNode* n) const
{
	return vectorDistance(m_weights, n->m_weights);
}

float ESOINNNode::maxDistanceToNeibs() const
{
	float maxDist = std::numeric_limits<float>::min();

	for (auto it = m_links.begin();
			it != m_links.end(); ++it)
	{
		const ESOINNNode* n = (*it).first;
		float dist = vectorDistance(m_weights, n->m_weights);
		if (dist > maxDist)
			maxDist = dist;
	}

	return maxDist;
}

float ESOINNNode::meanDistanceToNeibs() const
{
	if (m_links.empty())
		return 0;

	double mean = 0;
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		const ESOINNNode* n = (*it).first;
		mean += vectorDistance(m_weights, n->m_weights);
	}

	mean /= m_links.size();

	return mean;
}


void ESOINNNode::setSubClass(int32_t subClass)
{
	m_subClass = subClass;
}

int32_t ESOINNNode::subClass() const
{
	return m_subClass;
}

void ESOINNNode::setNeibsSubClass(int32_t subClass)
{
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;
		if (node->subClass() == UNKNOW_LABEL)
		{
			node->m_subClass = subClass;
			node->setNeibsSubClass(subClass);
		}
	}
}

void ESOINNNode::setNeibsSubClassFromApex(int32_t subClass, ESOINNNode* apex)
{
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;
		if (node->subClass() == UNKNOW_LABEL && node->m_density < apex->m_density)
		{
			node->m_subClass = subClass;
			node->setNeibsSubClassFromApex(subClass, node);
//			if (apex->realLabel() != UNKNOW_LABEL)
//			{
				//node->setRealLabel(apex->realLabel());
				//assert(node->realLabel() == UNKNOW_LABEL || apex->realLabel() == node->realLabel());
			//}
		}
	}
}

void ESOINNNode::setRealLabel(int32_t realLabel)
{
	m_realLabel = realLabel;
}

int32_t ESOINNNode::realLabel() const
{
	return m_realLabel;
}

double ESOINNNode::density() const
{
	return m_density;
}

void ESOINNNode::updateDensity(float meanDist)
{
	assert(m_winCount);
	m_s += 1.0 / ((1.0 + meanDist) * (1.0 + meanDist));
	assert(!std::isnan(m_density));
	//assert(m_density != 0);
	m_density = m_s / double(m_winCount);
	assert(!std::isinf(m_density));
	assert(!std::isnan(m_density));
}

void ESOINNNode::saveToStream(std::ostream &os)
{
	ut::write(os, m_dist);
	ut::write(os, m_s);
	ut::write(os, m_density);
	ut::write(os, m_subClass);
	ut::write(os, m_realLabel);
	ut::write(os, m_winCount);
	ut::write(os, m_id);

	ut::write(os, m_weights.size());
	for (float d : m_weights)
	{
		ut::write(os, d);
	}
}

void ESOINNNode::saveLinksToStream(std::ostream &stream)
{
	ut::write(stream , m_links.size());
	for (const auto & a : m_links)
	{
		ut::write(stream, a.first->m_id);
		ut::write(stream, a.second);
	}
}

void ESOINNNode::loadLinksFromStream(std::istream &fs, std::map<uint64_t, ESOINNNode*> &neurons)
{
	std::size_t size;
	ut::read(fs, size);
	for (std::size_t i = 0; i < size; ++i)
	{
		uint64_t id;
		uint16_t age;
		ut::read(fs, id);
		ut::read(fs, age);
		m_links[neurons[id]] = age;
	}
}

uint64_t ESOINNNode::nodeId() const
{
	return m_id;
}

bool ESOINNNode::operator==(const ESOINNNode &other) const
{
	bool firstResult = m_dist == other.m_dist &&
					   m_s == other.m_s &&
					   m_density == other.m_density &&
					   m_subClass == other.m_subClass &&
//					   (m_realLabel == other.m_realLabel) &&
					   m_winCount == other.m_winCount &&
					   m_id == other.m_id &&
					   m_weights == other.m_weights;

	if (!firstResult)
		return false;
	auto it1 = m_links.begin();
	auto it2 = other.m_links.begin();

	for (; it1 != m_links.end() && it2 != other.m_links.end(); ++it1, ++it2)
	{
		const ESOINNNode* e1 = (*it1).first;
		const ESOINNNode* e2 = (*it2).first;

		if (e1->m_id != e2->m_id || (*it1).second != (*it2).second)
			return false;
	}
	return true;
}

#ifdef BUILD_WITH_PNG_EXPORT_SUPPORT
void ESOINNNode::saveToPng(const std::string &path, int rows, int cols) const
{
	if ((rows * cols) != m_weights.size())
		throw std::runtime_error("Invalid rows/cols count!");
//        std::string fnm(folderPath);
//        std::string name = "RL=" + std::to_string(n->realLabel()) + "_";
//        name += "SL=" + std::to_string(n->subClass()) + "_";
//        name += std::to_string(i);
//        name += ".png";

  //  fnm.append(path);
	png::image<png::ga_pixel> image;
	image.resize(rows, cols);

	int p = 0;
	for (const float& w : m_weights)
	{
		unsigned char c = w * 255;
		int row = p / rows;
		int col = p % cols;
		image.set_pixel(col, row, c);
		++p;
	}
	image.write(path);
}
#endif

std::vector<uint64_t> ESOINNNode::idLinks() const
{
	std::vector<uint64_t> result;
	result.reserve(m_links.size());
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		ESOINNNode* node = it->first;
		result.push_back(node->nodeId());
	}
	return result;
}

uint32_t ESOINNNode::winCount() const
{
	return m_winCount;
}

int ESOINNNode::validateLinks() const
{
	int fails = 0;
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		const ESOINNNode* node = it->first;
		auto nit = node->m_links.find(const_cast<ESOINNNode*>(this));
		if (nit == node->m_links.end())
			++fails;
	}
	return fails;
}

void ESOINNNode::loadFromStream(std::istream & fs)
{
	ut::read(fs, m_dist);
	ut::read(fs, m_s);
	ut::read(fs, m_density);
	ut::read(fs, m_subClass);
	ut::read(fs, m_realLabel);
	ut::read(fs, m_winCount);
	ut::read(fs, m_id);

	std::size_t size;
	ut::read(fs, size);
	m_weights.resize(size);
	for (int i = 0; i < size; ++i)
	{
		ut::read(fs, m_weights[i]);
	}
}
