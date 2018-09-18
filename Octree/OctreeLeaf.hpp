#pragma once
#include "OctreeArray.hpp"
#include "OctreeCell.hpp"
#include "OctreeBranch.hpp"

/**
* outer node implementation of an octree cell.stores pointers to items.
*/
namespace hxa {
	class OctreeLeaf : public OctreeCell {
	private:
		OctreeArray<const void*> items_m;
	public:
		OctreeLeaf();
		OctreeLeaf(const OctreeLeaf*const leafs[8]);
	private:
		explicit OctreeLeaf(const void* pItem);
	public:
		virtual ~OctreeLeaf();
		OctreeLeaf(const OctreeLeaf&);
		OctreeLeaf& operator=(const OctreeLeaf&);
		virtual void  insertItem(const OctreeData& thisData, OctreeCell*& pThis, const void* pItem, const OctreeAgentV& agent);
		virtual bool  removeItem(OctreeCell*& pThis, const void* pItem,
			const int maxItemsPerCell, int& itemCount);
		virtual void  visit(const OctreeData& thisData, OctreeVisitorV& visitor) const;
		virtual OctreeCell* clone() const;
		virtual void  getInfo(int& byteSize, int& leafCount, int& itemCount, int& maxDepth) const;
		static void insertItemMaybeCreate(const OctreeData& cellData, OctreeCell*& pCell, const void* pItem, const OctreeAgentV& agent);
	};
}

