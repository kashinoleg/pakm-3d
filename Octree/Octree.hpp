#pragma once
#include "OctreeRoot.hpp"
#include "OctreeAgent.hpp"
#include "OctreeVisitor.hpp"

/**
* octree based spatial index.<br/><br/>
*
* client must define concrete derivatives of OctreeAgent<ItemType> and
* OctreeVisitor<ItemType>.<br/><br/>
*
* maxItemCountPerCell is ignored where maxLevelCount is reached.<br/><br/>
*
* the octree is cubical and axis aligned, partitions are axis aligned,
* partitions divide in half, each level partitions the previous level in all
* three axiss.<br/><br/>
*
* storage is contracted or expanded as needed by item insertion and removal.
* <br/><br/>
*
* (occupies, very approximately, 20 bytes per point item. maybe...)<br/><br/>
*
* octree is only an index: it points to client items, it does not manage
* storage of items themselves.<br/><br/>
*
* @see OctreeAgent
* @see OctreeVisitor
*
* @implementation
* the octree structure follows the Composite pattern.<br/><br/>
*
* this template wrapper ensures the items indexed by the octree and the agents
* and visitors used when accessing them are of matching types. all algorithmic
* work is delegated to OctreeRoot and OctreeCell derivatives in
* OctreeImplementation, which work with abstract base interfaces and void
* pointers.<br/><br/>
*
* for the insertion and removal commands, the agent provides an interface for
* the octree to query the typeless item, and for the visit query, the visitor
* provides callbacks to read tree nodes for carrying out the visit operation.
*/
namespace hxa {
	template<class TYPE>
	class Octree {
	private:
		OctreeRoot root_m;
	public:
		/**
		 * constructs a particular format of octree.<br/><br/>
		 * @parameters
		 * * sizeOfCube is desired length along a side<br/>
		 * * maxItemCountPerCell is desired max item pointers per leaf<br/>
		 * * maxLevelCount is desired max depth of tree<br/>
		 */
		Octree(const OctreeVector& positionOfLowerCorner, float sizeOfCube, int maxItemCountPerCell, int maxLevelCount);

		virtual ~Octree();
		Octree(const Octree&);
		Octree& operator=(const Octree&);


		/// commands -------------------------------------------------------------------
			/**
			 * add pointer(s) to the item to the octree.<br/><br/>
			 * (if an item has non-zero volume, it may have pointers in multiple
			 * cells.)<br/><br/>
			 * @return is the item inserted -- false if item not inside root bound
			 * @exceptions
			 * can throw storage allocation exceptions. in such cases the octree remains
			 * structurally ok, but the item will not be fully added, -- call this
			 * method again or the removeItem method.
			 * @see removeItem, OctreeAgent
			 */
		virtual bool  insertItem(const TYPE& item, const OctreeAgent<TYPE>& agent);
		/**
		 * removes pointer(s) to the item from the octree.<br/><br/>
		 * (if an item has non-zero volume, it may have pointers in multiple
		 * cells.)<br/><br/>
		 * @return is the item removed -- false if item wasn't present
		 * @exceptions
		 * can throw storage allocation exceptions. in such cases the octree remains
		 * structurally ok, but the item will not be fully removed, -- call this
		 * method again or the insertItem method.
		 * @see insertItem, OctreeAgent
		 */
		virtual bool  removeItem(const TYPE& item, const OctreeAgent<TYPE>& agent);


		/// queries --------------------------------------------------------------------
			/**
			 * execute a visit query operation.
			 * @see OctreeVisitor
			 */
		virtual void  visit(OctreeVisitor<TYPE>& visitor) const;

		/**
		 * reports if the octree is empty.
		 */
		virtual bool isEmpty() const;
		/**
		 * provides stats on the octree.<br/><br/>
		 * @parameters
		 * * byteSize is size in bytes<br/>
		 * * leafCount is number of leafs<br/>
		 * * itemRefCount is total number of item pointers in all leafs<br/>
		 * * maxDepth is deepest depth of tree<br/>
		 */
		virtual void getInfo(int& byteSize, int& leafCount, int& itemRefCount, int& maxDepth) const;

		/**
		 * gives the position supplied at construction.
		 */
		virtual const OctreeVector& getPosition() const;
		/**
		 * gives the size supplied at construction.
		 */
		virtual float getSize() const;
		/**
		 * gives the leaf pointer limit supplied at construction.
		 */
		virtual int getMaxItemCountPerCell() const;
	};

	template<class TYPE>
	inline Octree<TYPE>::Octree (const OctreeVector& position, const float sizeOfCube, const int maxItemCountPerCell, const int maxLevelCount)
		:root_m(position, sizeOfCube, maxItemCountPerCell, maxLevelCount) {
	}

	template<class TYPE>
	inline Octree<TYPE>::~Octree(){
	}

	template<class TYPE>
	inline Octree<TYPE>::Octree(const Octree& other)
		: root_m(other.root_m) {
	}

	template<class TYPE>
	inline Octree<TYPE>& Octree<TYPE>::operator=(const Octree& other) {
		root_m = other.root_m;
		return *this;
	}

	template<class TYPE>
	inline bool Octree<TYPE>::insertItem (const TYPE& item, const OctreeAgent<TYPE>& agent) {
		return root_m.insertItem(&item, agent);
	}

	template<class TYPE>
	inline bool Octree<TYPE>::removeItem(const TYPE& item, const OctreeAgent<TYPE>& agent) {
		return root_m.removeItem(&item);
	}

	template<class TYPE>
	inline void Octree<TYPE>::visit(OctreeVisitor<TYPE>& visitor) const {
		root_m.visit(visitor);
	}

	template<class TYPE>
	inline bool Octree<TYPE>::isEmpty() const {
		return root_m.isEmpty();
	}

	template<class TYPE>
	inline void Octree<TYPE>::getInfo(int& byteSize, int& leafCount, int& itemRefCount, int& maxDepth) const {
		root_m.getInfo(sizeof(*this), byteSize, leafCount, itemRefCount, maxDepth);
	}

	template<class TYPE>
	inline const OctreeVector& Octree<TYPE>::getPosition() const {
		return root_m.getPosition();
	}

	template<class TYPE>
	inline float Octree<TYPE>::getSize() const {
		return root_m.getSize();
	}

	template<class TYPE>
	inline int Octree<TYPE>::getMaxItemCountPerCell() const {
		return root_m.getMaxItemCountPerCell();
	}
}
