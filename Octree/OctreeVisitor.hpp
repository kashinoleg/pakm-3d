#pragma once

namespace hxa {
	/**
	* visitor abstract base, for client use with Octree.<br/><br/>
	*
	* client of Octree must define a concrete derivative of
	* OctreeVisitor<ItemType>.<br/><br/>
	*
	* this is a reversal of the Visitor pattern: it allows an operation to be
	* performed with the octree, except the octree is merely read from and it is
	* the visitor that is modified.<br/><br/>
	*
	* the visit methods are called by the tree nodes during the visit operation.
	* the parameters supply the cell and boundary info. the implementation can
	* call visit on the supplied cell.<br/><br/>
	*
	* the implementation of visitBranch needs to make the OctreeData to be given
	* in each call of visit.
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
	* @implementation
	* the ___V methods simply apply a type cast to void*s and forward to their
	* abstract counterparts.<br/><br/>
	*/
	template<class TYPE>
	class OctreeVisitor : public OctreeVisitorV {
	protected:
		OctreeVisitor() {}
	public:
		virtual ~OctreeVisitor() {}
	private:
		OctreeVisitor(const OctreeVisitor&) = delete;
		OctreeVisitor& operator=(const OctreeVisitor&) = delete;
	public:
		virtual void visitRootV(const OctreeCell* pRootCell, const OctreeData& octreeData) {
			visitRoot(pRootCell, octreeData);
		}
		virtual void visitBranchV(const OctreeCell* subCells[8], const OctreeData& octreeData) {
			visitBranch(subCells, octreeData);
		}
		virtual void visitLeafV(const OctreeArray<const void*>& items, const OctreeData& octreeData) {
			visitLeaf(reinterpret_cast<const OctreeArray<const TYPE*>&>(items), octreeData);
		}
	protected:
		/**
		* called by octree when visit traversal is at the root.<br/><br/>
		* to continue deeper, implementation calls visit on pRootCell, supplying
		* octreeData and *this as arguments.<br/><br/>
		* @see OctreeData
		*/
		virtual void visitRoot(const OctreeCell* pRootCell, const OctreeData& octreeData) = 0;
		/**
		* called by octree when visit traversal is at a branch.<br/><br/>
		* to continue deeper, implementation calls visit on any/all subcells,
		* supplying a new octreeData and *this as arguments. the new octreeData is
		* made with its constructor that will take the original octreeData and the
		* subcell index. check subcell values before use, since they can be null.
		* <br/><br/>
		* @see OctreeData
		*/
		virtual void visitBranch(const OctreeCell* subCells[8], const OctreeData& octreeData) = 0;
		/**
		* called by octree when visit traversal is at a leaf.<br/><br/>
		* @see OctreeData
		*/
		virtual void visitLeaf(const OctreeArray<const TYPE*>& items, const OctreeData& octreeData) = 0;
	};
}
