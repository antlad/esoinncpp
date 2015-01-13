#ifndef ESOINN_NODE_H
#define ESOINN_NODE_H

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cstdint>

const int32_t UNKNOW_LABEL = std::numeric_limits<int32_t>::min();

/*! Esoinn node class
 */
class ESOINNNode
{
public:
	/*! Constructor
	 */
	ESOINNNode();

	/*! Constructor
	 * \param weights Weights of node
	 * \param id Label
	 */
	ESOINNNode(const std::vector<float>& weights, uint64_t id);

	/*! Add link with other node
	 * \param n Other node
	 */
	void addLink(ESOINNNode* n);

	/*! Remove link with other node
	 * \param n Other node
	 */
	void removeLink(ESOINNNode* n);

	/*! Total win count of node
	 * \return Win count
	 */
	uint32_t winCount();

	/*! Increment total win count
	 */
	void incrementWinCount();

	/*! Calculate new input data vector
	 * \param x Data input vector
	 */
	void procInput(const std::vector<float>& x);

	/*! Total links count
	 * \return Links count
	 */
	std::size_t linksCount() const;

	/*! Calculate distance to other node
	 * \param n Other node
	 * \return Distance
	 */
	float distanceTo(ESOINNNode* n) const;

	/*! Return max distance to neibours
	 * \return Max distance to neibours
	 */
	float maxDistanceToNeibs() const;

	/*! Calculate and return mean distance to neibs
	 * \return Mean distance
	 */
	float meanDistanceToNeibs() const;

	/*! Set current sub class of node
	 * \param subClass Sub class
	 */
	void setSubClass(int32_t subClass);

	/*! Return current sub class of node
	 * \return Sub class
	 */
	int32_t subClass() const;

	/*! Recursively set up neibs sub class
	 * \param subClass
	 * \param apex Apex node
	 */
	void setNeibsSubClass(int32_t subClass, ESOINNNode* apex);

	/*! Set real label of node
	 * \param realLabel
	 */
	void setRealLabel(int32_t realLabel);

	/*! Return real label of node
	 * \return Real label of node
	 */
    int32_t realLabel() const;

	/*! Return current density of node
	 * \return Current density
	 */
	double density() const;

	/*! Calculate new density of node
	 * \param meanDist Mean density
	 */
	void updateDensity(float meanDist);

	/*! Move weight to input
	 * \param E1 Constant E1
	 * \param E2 Constant E2
	 * \param x Input vector
	 */
	void adaptWeight(float E1, float E2,
					 const std::vector<float>& x);

	/*! Increment node age
	 */
	void incrementLinksAge();

	/*! Remove links over max age
	 * \param maxAge Max age limit
	 */
	void clearOldLinks(int maxAge);

	/*! Return const reference of weights
	 * \return Const reference of weights
	 */
	const std::vector<float>& weights() const;

	/*! Current distance to input vector
	 * \return Distance to input vector
	 */
	float distance() const;

	/*! Overlapping list of nodes
	 * \return List of
	 */
	std::vector<ESOINNNode*> overlapedList();

	/*! Recursevly changing node and links sub class of node to new
	 * \param newSubClass
	 */
	void mergeWithNewSubClass(int32_t newSubClass);

	/*! Splitting invalid links
	 */
	void splitNoise();

	/*! Load node data from input stream
	 * \param stream Input stream
	 */
	void loadFromStream(std::istream & stream);

	/*! Save node data to output stream
	 * \param os Output stream
	 */
	void saveToStream(std::ostream & os);

	/*! Save node links to output stream
	 * \param stream Output sream
	 */
	void saveLinksToStream(std::ostream & stream);

	/*! Load node links of node from input stream
	 * \param stream
	 * \param neurons
	 */
	void loadLinksFromStream(std::istream & stream, std::map<uint64_t, ESOINNNode *> &neurons);

	/*! Return node id
	 * \return Node id
	 */
	uint64_t nodeId() const;

	/*! Remove back links from neibs nodes
	 */
	void destroy();

	/*! Compare operator
     * Only for debug use
	 * \param other Other node
	 * \return True if same
	 */
	bool operator==(const ESOINNNode & other) const;

    /*! Save weights into png file
     * Debuging function
     * \param path Path to png file
     * \param rows rows count
     * \param cols cols count
     */
    void saveToPng(const std::string & path, int rows, int cols) const;

private:
	float m_dist;								///< Current distance to input vector
	double m_s;									///< Current value of calculating mean density
	double m_density;							///< Current density of node
	int32_t m_subClass;							///< Sub class of node
	int32_t m_realLabel;						///< Real label of node
	uint32_t m_winCount;						///< Win count of node
	uint64_t m_id;								///< Id of node
    std::vector<float> m_weights;				///< Weight of node
    std::map<ESOINNNode*, uint16_t> m_links;	///< Links with other nodes
};

typedef std::shared_ptr<ESOINNNode> ESOINNNodePtr;

#endif
