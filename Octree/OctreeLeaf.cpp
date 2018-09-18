#include "OctreeLeaf.hpp"
using namespace hxa;

OctreeLeaf::OctreeLeaf()
	: items_m()
{
}


OctreeLeaf::OctreeLeaf
(
	const void* pItem
)
	: items_m()
{
	items_m.append(pItem);
}


OctreeLeaf::OctreeLeaf
(
	const OctreeLeaf*const leafs[8]
)
	: items_m()
{
	/// sum all items lengths
	int totalLength = 0;
	for (int i = 8; i-- > 0; )
	{
		const OctreeLeaf*const pLeaf = leafs[i];
		if (0 != pLeaf)
		{
			totalLength += pLeaf->items_m.getLength();
		}
	}

	/// prepare items array to hold all other items
	items_m.setLength(totalLength);

	/// copy items arrays
	const void** pElement = items_m.getMemory();
	for (int i = 0; i < 8; ++i)
	{
		const OctreeLeaf*const pLeaf = leafs[i];
		if (0 != pLeaf)
		{
			const void** pOtherElement = pLeaf->items_m.getMemory();
			const void** pOtherEnd = pOtherElement + pLeaf->items_m.getLength();
			for (; pOtherElement < pOtherEnd; ++pOtherElement, ++pElement)
			{
				*pElement = *pOtherElement;
			}
		}
	}
}


OctreeLeaf::~OctreeLeaf() {
}

OctreeLeaf::OctreeLeaf(const OctreeLeaf& other)
	: OctreeCell()
	, items_m(other.items_m)
{
}


OctreeLeaf& OctreeLeaf::operator=(const OctreeLeaf& other) {
	items_m = other.items_m;
	return *this;
}

void OctreeLeaf::insertItem
(
	const OctreeData&   thisData,
	OctreeCell*&        pThis,
	const void* const   pItem,
	const OctreeAgentV& agent
)
{
	/// check if item already present
	bool isAlreadyPresent = false;
	for (int i = items_m.getLength(); (i-- > 0) & !isAlreadyPresent; )
	{
		isAlreadyPresent |= (items_m[i] == pItem);
	}

	/// only insert if item not already present
	if (!isAlreadyPresent)
	{
		/// check if leaf should be subdivided
		if (!thisData.isSubdivide(items_m.getLength() + 1))
		{
			/// append item to collection
			items_m.append(pItem);
		}
		else
		{
			/// subdivide by making branch and adding items to it
			OctreeCell*const pBranch = new OctreeBranch(thisData, items_m, pItem, agent);

			/// replace this with branch
			delete pThis;
			pThis = pBranch;
		}
	}
}


bool OctreeLeaf::removeItem
(
	OctreeCell*&      pThis,
	const void* const pItem,
	const int,//maxItemsPerCell,
	int&            itemCount
)
{
	bool isRemoved = false;

	/// loop through items
	for (int i = 0; i < items_m.getLength(); )
	{
		/// check if item is present
		if (items_m[i] == pItem)
		{
			/// remove item
			items_m.remove(i);
			isRemoved = true;
		}
		else
		{
			++i;
		}
	}

	itemCount += items_m.getLength();

	/// check if leaf is now empty
	if (items_m.isEmpty())
	{
		/// remove this leaf
		delete pThis;
		pThis = 0;
	}

	return isRemoved;
}

void OctreeLeaf::visit(const OctreeData& thisData, OctreeVisitorV& visitor) const {
	visitor.visitLeafV(items_m, thisData);
}

OctreeCell* OctreeLeaf::clone() const {
	return new OctreeLeaf(*this);
}

void OctreeLeaf::getInfo
(
	int& byteSize,
	int& leafCount,
	int& itemCount,
	int& maxDepth
) const {
	byteSize += sizeof(*this) + (items_m.getLength() * sizeof(void*));
	++leafCount;
	itemCount += items_m.getLength();
	++maxDepth;
}

void OctreeLeaf::insertItemMaybeCreate(const OctreeData& cellData,
	OctreeCell*& pCell, const void* const pItem, const OctreeAgentV& agent) {
	/// check cell exists
	if (pCell == NULL) {
		/// make leaf, adding item
		OctreeCell*const pLeaf = new OctreeLeaf(pItem);
		/// replace cell with leaf
		delete pCell;
		pCell = pLeaf;
	}
	else {
		/// forward to existing cell
		pCell->insertItem(cellData, pCell, pItem, agent);
	}
}
