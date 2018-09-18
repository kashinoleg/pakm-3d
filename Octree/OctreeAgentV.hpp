#pragma once
#include "OctreeVector.hpp"

/**
* agent abstract base, for Octree implementation use.
* return value of getSubcellOverlapsV is 8 bits, each bit is a bool
* corresponding to a subcell, the high bit for subcell 7, the low bit for subcell 0.
*/
namespace hxa {
	class OctreeAgentV {
	protected:
		OctreeAgentV() {}
	public:
		virtual ~OctreeAgentV() {}
	private:
		OctreeAgentV(const OctreeAgentV&) = delete;
		OctreeAgentV& operator=(const OctreeAgentV&) = delete;
	public:
		virtual bool  isOverlappingCellV(const void* pItem, const OctreeVector& lowerCorner,
			const OctreeVector& upperCorner) const = 0;
		virtual int getSubcellOverlapsV(const void* pItem, const OctreeVector& lower,
			const OctreeVector& middle, const OctreeVector& upper) const = 0;
	};
}
