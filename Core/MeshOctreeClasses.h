#pragma once
#include <list>
#include <set>
#include "Octree.hpp"
#include "mymath.h"

namespace TexGen {
	using namespace hxa;
	using namespace std;

	class CMesh;

	/// Octree agent used to add indexed nodes to an octree
	class COctreeAgentNode : public OctreeAgent<pair<int, XYZ> >
	{
	public:
		COctreeAgentNode(double dTolerance = 0) : m_dTolerance(dTolerance) {};
		~COctreeAgentNode() {};

	protected:
		bool isOverlappingCell(const pair<int, XYZ>& item, const OctreeVector& lowerCorner, const OctreeVector& upperCorner) const;
		int getSubcellOverlaps(const pair<int, XYZ>& item, const OctreeVector& lower, const OctreeVector& middle, const OctreeVector& upper) const;

		double m_dTolerance;
	};

	/// Octree visitor used to merge nodes together within a given tolerance
	class COctreeVisitorMergeNodes : public OctreeVisitor<pair<int, XYZ> >
	{
	public:
		COctreeVisitorMergeNodes(CMesh &Mesh, double dTolerance = 0) : 
			m_Mesh(Mesh), m_iNumMerged(0), m_dToleranceSquared(dTolerance*dTolerance) {};
		~COctreeVisitorMergeNodes() {};

		int GetNumMerged() { return m_iNumMerged; }

	protected:
		void visitRoot(const OctreeCell* pRootCell, const OctreeData& octreeData);
		void visitBranch(const OctreeCell* subCells[8], const OctreeData& octreeData);
		void visitLeaf(const OctreeArray<const pair<int, XYZ>*>& items, const OctreeData& octreeData);

		CMesh &m_Mesh;
		int m_iNumMerged;
		double m_dToleranceSquared;
		vector<vector<int*> > m_NodeElementReferences;
//		vector<bool> m_DeletedNodes;
		set<int> m_DeletedNodes;
	};

	/// Elements of a given, is used in conjunction with COctreeAgentElement
	struct MESH_ELEMENT
	{
		vector<int> NodeIndices;
		int iIndex;
	};

	/// Class to assist in storing mesh elements in an octree
	class CElementsOctree {
	protected:
		Octree<MESH_ELEMENT> *m_pOctree;
		list<MESH_ELEMENT> m_OctreeElements;
	public:
		CElementsOctree() : m_pOctree(NULL) {}
		CElementsOctree(const CElementsOctree &CopyMe) {
			m_pOctree = NULL;
			if (CopyMe.m_pOctree) {
				m_pOctree = new Octree<MESH_ELEMENT>(*CopyMe.m_pOctree);
			}
			m_OctreeElements = CopyMe.m_OctreeElements;
		}
		~CElementsOctree() {
			if (m_pOctree) {
				delete m_pOctree;
			}
		}
		Octree<MESH_ELEMENT> *GetOctree() {
			return m_pOctree;
		}
		const Octree<MESH_ELEMENT> *GetOctree() const {
			return m_pOctree;
		}
		list<MESH_ELEMENT> &GetElements() {
			return m_OctreeElements;
		}
		const list<MESH_ELEMENT> &GetElements() const {
			return m_OctreeElements;
		}
	};

	/// Octree agent used to add elements to an octree
	class COctreeAgentElement : public OctreeAgent<MESH_ELEMENT>
	{
	public:
		COctreeAgentElement(CMesh &Mesh) : m_Mesh(Mesh) {};
		~COctreeAgentElement() {};

	protected:
		bool isOverlappingCell(const MESH_ELEMENT& item, const OctreeVector& lowerCorner, const OctreeVector& upperCorner) const;
		int getSubcellOverlaps(const MESH_ELEMENT& item, const OctreeVector& lower, const OctreeVector& middle, const OctreeVector& upper) const;

		CMesh &m_Mesh;
	};

	/// Octree visitor used to get a list of elements near given line
	class COctreeVisitorElementNearLine : public OctreeVisitor<MESH_ELEMENT>
	{
	public:
		/// Get a list of elements that are likely to intersect with the given line
		/**
		\param P1 Origin of the line
		\param P2 End of the line
		\param Elements A list of elements near the line
		\param TrimLine If true the line ends at the origin and end respecitively
		*/
		COctreeVisitorElementNearLine(const XYZ &P1, const XYZ &P2, list< vector<int> > &Elements, pair<bool, bool> TrimLine) : m_P1(P1), m_P2(P2), m_Elements(Elements), m_TrimLine(TrimLine) {}
		~COctreeVisitorElementNearLine() {};

	protected:
		void visitRoot(const OctreeCell* pRootCell, const OctreeData& octreeData);
		void visitBranch(const OctreeCell* subCells[8], const OctreeData& octreeData);
		void visitLeaf(const OctreeArray<const MESH_ELEMENT*>& items, const OctreeData& octreeData);

		const XYZ &m_P1;
		const XYZ &m_P2;
		list< vector<int> > &m_Elements;
		pair<bool, bool> m_TrimLine;
		vector<bool> m_Used;
	};

};	// namespace TexGen
