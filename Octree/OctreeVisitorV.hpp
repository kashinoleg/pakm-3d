#pragma once
#include "OctreeArray.hpp"
#include "OctreeData.hpp"

namespace hxa {
	class OctreeCell;
	/**
	* visitor abstract base, for Octree implementation use.<br/><br/>
	*
	* subcell numbering:
	* <pre>
	*    y z       6 7
	*    |/   2 3  4 5
	*     -x  0 1
	* </pre>
	* in binary:
	* <pre>
	*    y z           110 111
	*    |/   010 011  100 101
	*     -x  000 001
	* </pre>
	*
	* @see OctreeCell
	* @see OctreeBranch
	* @see OctreeLeaf
	*/
	class OctreeVisitorV {
	protected:
		OctreeVisitorV() {}
	public:
		virtual ~OctreeVisitorV() {}
	private:
		OctreeVisitorV(const OctreeVisitorV&);
		OctreeVisitorV& operator=(const OctreeVisitorV&) = delete;
	public:
		virtual void  visitRootV(const OctreeCell* pRootCell, const OctreeData& octreeData) = 0;
		virtual void  visitBranchV(const OctreeCell* subCells[8], const OctreeData& octreeData) = 0;
		virtual void  visitLeafV(const OctreeArray<const void*>& items, const OctreeData& octreeData) = 0;
	};
}
