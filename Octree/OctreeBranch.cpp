#include "OctreeBranch.hpp"

using namespace hxa;

OctreeBranch::OctreeBranch() {
	OctreeBranch::zeroSubCells();
}

OctreeBranch::OctreeBranch(const OctreeData& thisData, const OctreeArray<const void*>& items,
		const void* const pItem, const OctreeAgentV& agent) {
	OctreeBranch::zeroSubCells();
	try {
		OctreeCell* pNotUsed = 0;
		/// insert items
		for (int j = items.getLength(); j-- > 0; ) {
			OctreeBranch::insertItem(thisData, pNotUsed, items[j], agent);
		}
		OctreeBranch::insertItem(thisData, pNotUsed, pItem, agent);
	}
	catch (...) {
		/// delete any allocated cells
		this->~OctreeBranch();
		throw;
	}
}

OctreeBranch::~OctreeBranch() {
	for (int i = 8; i-- > 0; ) {
		delete subCells_m[i];
	}
}

OctreeBranch::OctreeBranch(const OctreeBranch& other) : OctreeCell() {
	OctreeBranch::zeroSubCells();
	try {
		for (int i = 8; i-- > 0; ) {
			subCells_m[i] = OctreeCell::cloneNonZero(other.subCells_m[i]);
		}
	}
	catch (...) {
		/// delete any allocated cells
		this->~OctreeBranch();
		throw;
	}
}

OctreeBranch& OctreeBranch::operator=(const OctreeBranch& other) {
	if (&other != this) {
		for (int i = 8; i-- > 0; ) {
			delete subCells_m[i];
			subCells_m[i] = 0;
			subCells_m[i] = OctreeCell::cloneNonZero(other.subCells_m[i]);
		}
	}
	return *this;
}

void OctreeBranch::insertItem(const OctreeData&   thisData,
		OctreeCell*&, const void* const pItem, const OctreeAgentV& agent) {
	/// get subcell-item overlaps flags
	const OctreeBound& bound = thisData.getBound();
	const int overlaps = agent.getSubcellOverlapsV(pItem,
		bound.getLowerCorner(), bound.getCenter(), bound.getUpperCorner());

	/// loop through sub cells
	for (int i = 8; i-- > 0; ) {
		/// check if sub cell is overlapped by item
		if ((overlaps >> i) & 1)
		{
			/// make sub cell data
			const OctreeData subCellData(thisData, i);

			/// add item to sub cell
			OctreeLeaf::insertItemMaybeCreate(subCellData, subCells_m[i],
				pItem, agent);
		}
	}
}


bool OctreeBranch::removeItem
(
	OctreeCell*&      pThis,
	const void* const pItem,
	const int       maxItemsPerCell,
	int&            itemCount
)
{
	bool  isRemoved = false;
	int branchItemCount = 0;

	/// loop through sub cells
	for (int i = 8; i-- > 0; )
	{
		/// remove item from non-null sub cell
		OctreeCell*& pSubCell = subCells_m[i];
		if (pSubCell != 0)
		{
			isRemoved |= pSubCell->removeItem(pSubCell, pItem, maxItemsPerCell,
				branchItemCount);
		}
	}

	itemCount += branchItemCount;

	/// decide whether to collapse this branch
	if (branchItemCount > 0)
	{
		/// collapse to leaf
		if (branchItemCount <= maxItemsPerCell)
		{
			/// all subcells *will* be leafs!
			/// because:
			/// a) if a branch has below it less item refs than the threshold,
			///    it collapses to a leaf (this function!)
			/// b) the total of item refs below this branch in the tree is less
			///    than the threshold
			/// c) therefore the total of item refs in any branch below this
			///    cell will be less than the threshold
			/// d) branchs below this cell will be collapsed before this branch
			///    (because the recursive 'removeItem' call is before the
			///    collapsing code)
			/// so: if this branch will collapse to a leaf, then all its sub
			/// branchs (direct and indirect) will collapse to leafs, and that
			/// will happen before this branch.
			OctreeCell*const pLeaf = new OctreeLeaf(
				reinterpret_cast<OctreeLeaf**>(subCells_m));

			delete pThis;
			pThis = pLeaf;
		}
	}
	else
	{
		/// delete
		delete pThis;
		pThis = 0;
	}

	return isRemoved;
}




/// queries --------------------------------------------------------------------
void OctreeBranch::visit
(const OctreeData& thisData, OctreeVisitorV& visitor) const {
	visitor.visitBranchV(const_cast<const OctreeCell**>(subCells_m), thisData);
}

OctreeCell* OctreeBranch::clone() const {
	return new OctreeBranch(*this);
}

void OctreeBranch::getInfo
(
	int& byteSize,
	int& leafCount,
	int& itemCount,
	int& maxDepth
) const
{
	byteSize += sizeof(*this);

	const int thisDepth = maxDepth + 1;

	for (int i = 8; i-- > 0; )
	{
		const OctreeCell*const pSubCell = subCells_m[i];
		if (pSubCell != 0)
		{
			int depth = thisDepth;
			pSubCell->getInfo(byteSize, leafCount, itemCount, depth);

			if (maxDepth < depth) {
				maxDepth = depth;
			}
		}
	}
}

/// implementation -------------------------------------------------------------
void OctreeBranch::zeroSubCells() {
	for (int i = 8; i-- > 0; ) {
		subCells_m[i] = 0;
	}
}
