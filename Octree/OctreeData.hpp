#pragma once
#include "OctreeDimensions.hpp"
#include "OctreeBound.hpp"

namespace hxa {
	/**
	* global and local octree cell data.<br/><br/>
	*
	* constant.<br/><br/>
	*
	* to be made during each level of tree descent, so storage is avoided, except
	* to hold one at the root.<br/><br/>
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
	*/
	class OctreeData {
	private:
		/// local to cell
		OctreeBound bound_m;
		int       level_m;
		/// global for octree
		const OctreeDimensions* pDimensions_m;
	public:
		explicit OctreeData(const OctreeDimensions& dimensions);
		OctreeData(const OctreeData& parentCellData, int subCellIndex);
		OctreeData(const OctreeData&, const OctreeDimensions&);
		~OctreeData() {};
		OctreeData(const OctreeData&);
		OctreeData& operator=(const OctreeData&);
		const OctreeBound& getBound() const {
			return bound_m;
		}
		const OctreeDimensions& getDimensions() const {
			return *pDimensions_m;
		}
		bool isSubdivide(const int itemCount) const {
			return pDimensions_m->isSubdivide(itemCount, level_m);
		}
	};
}
