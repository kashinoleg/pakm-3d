#pragma once
#include "OctreeArray.hpp"
#include "OctreeRoot.hpp"
#include "OctreeCell.hpp"

/**
* inner node implementation of an octree cell.<br/><br/>
*
* stores pointers to eight child cells.
*
* @invariants
* subCells_m elements can be null, or point to an OctreeCell instance.
*/
namespace hxa {
	class OctreeBranch : public OctreeCell {
	private:
		OctreeCell* subCells_m[8];
	protected:
		virtual void  zeroSubCells();
	public:
		OctreeBranch();
		OctreeBranch(const OctreeData& thisData, const OctreeArray<const void*>& items, const void* const pItem, const OctreeAgentV& agent);
		~OctreeBranch() override final;
		OctreeBranch(const OctreeBranch&);
		OctreeBranch& operator=(const OctreeBranch&);

		void insertItem(const OctreeData& thisData, OctreeCell*& pThis, const void* pItem, const OctreeAgentV& agent) override final;
		bool removeItem(OctreeCell*& pThis, const void* pItem, const int maxItemsPerCell, int& itemCount) override final;
		void visit(const OctreeData& thisData, OctreeVisitorV& visitor) const override final;
		OctreeCell* clone() const final;
		void  getInfo(int& byteSize, int& leafCount, int& itemCount, int& maxDepth) const override final;
	};
}
