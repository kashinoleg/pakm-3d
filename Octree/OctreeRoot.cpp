#include "OctreeRoot.hpp"

using namespace hxa;

OctreeRoot::OctreeRoot(const OctreeVector& position, const float sizeOfCube,
		const int maxItemsPerCell, const int maxLevelCount) {
	dimensions_m = OctreeDimensions();
	dimensions_m.setPosition(position);
	dimensions_m.setSize(sizeOfCube);
	dimensions_m.setMaxItemCountPerCell(maxItemsPerCell);
	dimensions_m.setMaxLevel(maxLevelCount);
	pRootCell_m = NULL;
}

OctreeRoot::~OctreeRoot() {
	delete pRootCell_m;
}

OctreeRoot::OctreeRoot(const OctreeRoot& other) {
	dimensions_m = other.dimensions_m;
	pRootCell_m = OctreeCell::cloneNonZero(other.pRootCell_m);
}

OctreeRoot& OctreeRoot::operator=(const OctreeRoot& other) {
	if (&other != this) {
		delete pRootCell_m;
		pRootCell_m = NULL;
		pRootCell_m = OctreeCell::cloneNonZero(other.pRootCell_m);
		dimensions_m = other.dimensions_m;
	}
	return *this;
}

bool OctreeRoot::insertItem(const void* const pItem, const OctreeAgentV& agent) {
	bool isInserted = false;
	/// make data
	const OctreeData data(dimensions_m);
	/// check if item overlaps root cell
	if (agent.isOverlappingCellV(pItem, data.getBound().getLowerCorner(), data.getBound().getUpperCorner())) {
		OctreeLeaf::insertItemMaybeCreate(data, pRootCell_m, pItem, agent);
		isInserted = true;
	}
	return isInserted;
}

bool OctreeRoot::removeItem(const void* const pItem) {
	bool isRemoved = false;
	if (pRootCell_m != NULL) {
		int unusedBranchItemCount = 0;
		isRemoved = pRootCell_m->removeItem(pRootCell_m, pItem, dimensions_m.getMaxItemCountPerCell(), unusedBranchItemCount);
	}
	return isRemoved;
}

void OctreeRoot::visit(OctreeVisitorV& visitor) const {
	/// make data
	const OctreeData data(dimensions_m);
	visitor.visitRootV(pRootCell_m, data);
}

bool OctreeRoot::isEmpty() const {
	return pRootCell_m == NULL;
}

void OctreeRoot::getInfo(const int rootWrapperByteSize, int& byteSize,
		int& leafCount, int& itemCount, int& maxDepth) const {
	byteSize = 0;
	leafCount = 0;
	itemCount = 0;
	maxDepth = 0;
	if (pRootCell_m != NULL) {
		pRootCell_m->getInfo(byteSize, leafCount, itemCount, maxDepth);
	}
	byteSize += rootWrapperByteSize;
}

const OctreeVector& OctreeRoot::getPosition() const {
	return dimensions_m.getPosition();
}

float OctreeRoot::getSize() const {
	return dimensions_m.getSize();
}

int OctreeRoot::getMaxItemCountPerCell() const {
	return dimensions_m.getMaxItemCountPerCell();
}