#pragma once
#include "OctreeArray.hpp"
#include "OctreeCell.hpp"
#include "OctreeLeaf.hpp"

/**
* implementation class for the Octree template.
*
* @invariants
* pRootCell_m can be null, or point to an OctreeCell instance.<br/><br/>
*
* at construction, pRootCell_m is set to a legal value.<br/>
* at destruction, pRootCell_m is deleted.<br/>
* whenever pRootCell_m is modified, it must be deleted then set to a legal
* value.<br/>
* a legal value is: either 0, or the value from invocation of 'new'.
*/
namespace hxa {
	class OctreeRoot {
	private:
		OctreeDimensions dimensions_m;
		OctreeCell* pRootCell_m;
	public:
		OctreeRoot(const OctreeVector& position, float sizeOfCube, int maxItemsPerCell, int maxLevelCount);
		~OctreeRoot();
		OctreeRoot(const OctreeRoot&);
		OctreeRoot& operator=(const OctreeRoot&);
		bool insertItem(const void* pItem, const OctreeAgentV& agent);
		bool removeItem(const void* pItem);
		void  visit(OctreeVisitorV& visitor) const;
		bool isEmpty() const;
		void getInfo(int rootWrapperByteSize, int& byteSize, int& leafCount, int& itemCount, int& maxDepth) const;
		const OctreeVector& getPosition() const;
		float getSize() const;
		int getMaxItemCountPerCell() const;
	};
}
