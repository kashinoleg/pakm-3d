#pragma once

/**
* agent abstract base, for client use with Octree.<br/><br/>
*
* client of Octree must define a concrete derivative of
* OctreeAgent<ItemType>.<br/><br/>
*
* this is similar to a proxy: its an intermediary for an octree to query
* its typeless subject items, when inserting or removing.<br/><br/>
*
* the overlap methods are to determine an items relation to a cell or cells,
* for insertion or removal. the parameters supply the bounds of the cell.
* <br/><br/>
*
* return value of getSubcellOverlaps is 8 bits, each bit is a bool
* corresponding to a subcell, the high bit for subcell 7, the low bit for
* subcell 0.<br/><br/>
*
* @implementation
* the ___V methods simply apply a type cast to void*s and forward to their
* abstract counterparts.<br/><br/>
*
* an octree requires its contained items to provide positional info. but
* requiring the item classes to implement an OctreeItem interface would
* impose a direct interface change on every prospective item type, and enlarge
* their instances with a vptr.<br/><br/>
*
* instead, this agent transfers the octree related interface/implementation
* away from the item type into a separate class. the octree can now hold void
* pointers to items and call the agent to query them indirectly.<br/><br/>
*/
namespace hxa {
	template<class TYPE>
	class OctreeAgent : public OctreeAgentV {
	protected:
		OctreeAgent() {}
	public:
		virtual ~OctreeAgent() {}
	private:
		OctreeAgent(const OctreeAgent&) = delete;
		OctreeAgent& operator=(const OctreeAgent&) = delete;
	public:
		virtual bool  isOverlappingCellV(const void* pItem, const OctreeVector& lowerCorner, const OctreeVector& upperCorner) const {
			bool is = false;
			if (pItem != NULL) {
				is = isOverlappingCell(*reinterpret_cast<const TYPE*>(pItem), lowerCorner, upperCorner);
			}
			return is;
		}
		virtual int getSubcellOverlapsV(const void* pItem, const OctreeVector& lower,
				const OctreeVector& middle, const OctreeVector& upper) const {
			int ov = 0;
			if (pItem != NULL) {
				ov = getSubcellOverlaps(*reinterpret_cast<const TYPE*>(pItem), lower, middle, upper);
			}
			return ov;
		}
	protected:
		/**
		* called by octree to get relation of item to cell.
		*/
		virtual bool  isOverlappingCell(const TYPE& item, const OctreeVector& lowerCorner,
			const OctreeVector& upperCorner) const = 0;
		/**
		* called by octree to get relation of item to subcell octants.<br/><br/>
		* allows more efficent calculation, since boundaries are shared. but client
		* could implement by delegating to isOverlappingCell.
		* @return
		* 8 bits, each a bool corresponding to a subcell, the high bit for subcell
		* 7, the low bit for subcell 0.<br/><br/>
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
		virtual int getSubcellOverlaps(const TYPE& item, const OctreeVector& lowerCorner,
			const OctreeVector& middlePoint, const OctreeVector& upperCorner) const = 0;
	};
}
