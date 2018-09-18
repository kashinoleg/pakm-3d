#pragma once
#include "OctreeVector.hpp"

namespace hxa {
	/**
	* geometric data for the bound of an octree cell.<br/><br/>
	*
	* constant.<br/><br/>
	*
	* radius is that of the circumsphere.<br/><br/>
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
	class OctreeBound {
	private:
		OctreeVector positionOfLowerCorner_m;
		OctreeVector positionOfUpperCorner_m;
		OctreeVector center_m;
		float    circumSphereRadius_m;
	public:
		OctreeBound();
		OctreeBound(const OctreeVector& positionOfLowerCorner, float size);
		OctreeBound(const OctreeBound& parentCellBound, int subCellIndex);
		~OctreeBound() {}
		OctreeBound(const OctreeBound&);
		OctreeBound& operator=(const OctreeBound&);
		const OctreeVector& getLowerCorner() const {
			return positionOfLowerCorner_m;
		}
		const OctreeVector& getUpperCorner() const {
			return positionOfUpperCorner_m;
		}
		const OctreeVector& getCenter() const {
			return center_m;
		}
		float getRadius() const {
			return circumSphereRadius_m;
		}
	};
}
