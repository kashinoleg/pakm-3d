#pragma once
#include "OctreeArray.hpp"
#include "OctreeVisitorV.hpp"
#include "OctreeAgentV.hpp"

/**
* abstract base for Composite types, for implementing Octree nodes.
*/
namespace hxa {
	class OctreeCell {
	protected:
		OctreeCell() {}
	public:
		virtual ~OctreeCell() {}
	private:
		OctreeCell(const OctreeCell&) = delete;
		OctreeCell& operator=(const OctreeCell&) = delete;
	public:
		virtual void insertItem(const OctreeData& thisData, OctreeCell*& pThis, const void* pItem, const OctreeAgentV& agent) = 0;
		virtual bool removeItem(OctreeCell*& pThis, const void* pItem, const int maxItemsPerCell, int& itemCount) = 0;
		virtual void visit(const OctreeData& thisData, OctreeVisitorV& visitor) const = 0;
		virtual OctreeCell* clone() const = 0;
		virtual void getInfo(int& byteSize, int& leafCount, int& itemCount, int& maxDepth) const = 0;
		static OctreeCell* cloneNonZero(const OctreeCell* pOriginal) {
			if (pOriginal != NULL) {
				return pOriginal->clone();
			}
			else {
				return NULL;
			}
		}
	};
}