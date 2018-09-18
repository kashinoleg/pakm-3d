#include "MeshOctreeClasses.h"
#include "Mesh.h"

using namespace TexGen;

bool COctreeAgentNode::isOverlappingCell(const pair<int, XYZ>& item, const OctreeVector& lowerCorner, const OctreeVector& upperCorner) const
{
	return (item.second.x >= lowerCorner.getX() - m_dTolerance && item.second.y >= lowerCorner.getY() - m_dTolerance && item.second.z >= lowerCorner.getZ() - m_dTolerance &&
			item.second.x <= upperCorner.getX() + m_dTolerance && item.second.y <= upperCorner.getY() + m_dTolerance && item.second.z <= upperCorner.getZ() + m_dTolerance);
}

int COctreeAgentNode::getSubcellOverlaps(const pair<int, XYZ>& item, const OctreeVector& lower, const OctreeVector& middle, const OctreeVector& upper) const
{
	/// efficiency could (probably) be improved by doing minimal necessary checks
	/// against the dividing bounds, instead of repeatedly delegating to
	/// isOverlappingCell().

	int flags = 0;

	const OctreeVector* lowMidPoints[]  = { &lower, &middle };
	const OctreeVector* midHighPoints[] = { &middle, &upper };

	for (int i = 8;  i-- > 0; )
	{
		OctreeVector lowerCorner( lowMidPoints[ i       & 1]->getX(),
		                      lowMidPoints[(i >> 1) & 1]->getY(),
		                      lowMidPoints[(i >> 2) & 1]->getZ() );
		OctreeVector upperCorner( midHighPoints[ i       & 1]->getX(),
		                      midHighPoints[(i >> 1) & 1]->getY(),
		                      midHighPoints[(i >> 2) & 1]->getZ() );
		flags |= int(isOverlappingCell( item, lowerCorner, upperCorner )) << i;
	}

	return flags;
}


void COctreeVisitorMergeNodes::visitRoot(const OctreeCell* pRootCell, const OctreeData& octreeData)
{
	if (pRootCell != 0)
	{
		m_Mesh.GetNodeElementReferences(m_NodeElementReferences);
//		m_DeletedNodes.resize(m_Mesh.GetNumNodes(), false);

		pRootCell->visit(octreeData, *this);

//		cout << "Num nodes removed " << 
		m_Mesh.DeleteNodes(m_DeletedNodes);
//		cout << endl;
	}
}

void COctreeVisitorMergeNodes::visitBranch(const OctreeCell* subCells[8], const OctreeData& octreeData)
{
	for (int i = 0; i < 8; ++i)
	{
		const OctreeCell* pSubCell = subCells[i];
		if(pSubCell != 0)
		{
			const OctreeData subCellData(octreeData, i);
			pSubCell->visit(subCellData, *this);
		}
	}
}

void COctreeVisitorMergeNodes::visitLeaf(const OctreeArray<const pair<int, XYZ>*>& items, const OctreeData& octreeData)
{
	int i, j;
	int iNumItems = items.getLength();
	for (i=0; i<iNumItems; ++i)
	{
		// A possible optimisation would be j = i+1. However this causes nodes not to be merged correctly in rare cases, please leave it like this!
		for (j=0; j<iNumItems; ++j)
		{
			if (i == j)
				continue;
			if (GetLengthSquared(items[i]->second, items[j]->second) < m_dToleranceSquared)
			{
				++m_iNumMerged;
				if (items[i]->first < items[j]->first)
				{
					m_Mesh.ChangeNodeIndices(items[i]->first, items[j]->first, m_NodeElementReferences);
//					m_DeletedNodes[items[j]->first] = true;
					m_DeletedNodes.insert(items[j]->first);
				}
				else if (items[i]->first > items[j]->first)
				{
					m_Mesh.ChangeNodeIndices(items[j]->first, items[i]->first, m_NodeElementReferences);
//					m_DeletedNodes[items[i]->first] = true;
					m_DeletedNodes.insert(items[i]->first);
				}
				else
					assert(false);
			}
		}
	}
}

bool COctreeAgentElement::isOverlappingCell(const MESH_ELEMENT& item, const OctreeVector& lowerCorner, const OctreeVector& upperCorner) const
{
	vector<int>::const_iterator itNodeIndex;
	XYZ Min, Max;
	for (itNodeIndex = item.NodeIndices.begin(); itNodeIndex != item.NodeIndices.end(); ++itNodeIndex)
	{
		cout << *itNodeIndex << endl;
		if (itNodeIndex == item.NodeIndices.begin())
			Min = Max = m_Mesh.GetNode(*itNodeIndex);
		else
		{
			Min = TexGen::Min(Min, m_Mesh.GetNode(*itNodeIndex));
			Max = TexGen::Max(Max, m_Mesh.GetNode(*itNodeIndex));
		}
	}
	return (Max.x >= lowerCorner.getX() && Max.y >= lowerCorner.getY() && Max.z >= lowerCorner.getZ() &&
			Min.x <= upperCorner.getX() && Min.y <= upperCorner.getY() && Min.z <= upperCorner.getZ());
}

int COctreeAgentElement::getSubcellOverlaps(const MESH_ELEMENT& item, const OctreeVector& lower, const OctreeVector& middle, const OctreeVector& upper) const
{
	vector<int>::const_iterator itNodeIndex;
	XYZ Min, Max;
	for (itNodeIndex = item.NodeIndices.begin(); itNodeIndex != item.NodeIndices.end(); ++itNodeIndex)
	{
		cout << *itNodeIndex << endl;
		if (itNodeIndex == item.NodeIndices.begin())
			Min = Max = m_Mesh.GetNode(*itNodeIndex);
		else
		{
			Min = TexGen::Min(Min, m_Mesh.GetNode(*itNodeIndex));
			Max = TexGen::Max(Max, m_Mesh.GetNode(*itNodeIndex));
		}
	}

	int flags = 0;

	const OctreeVector* lowMidPoints[]  = { &lower, &middle };
	const OctreeVector* midHighPoints[] = { &middle, &upper };

	for (int i = 8;  i-- > 0; )
	{
		OctreeVector lowerCorner( lowMidPoints[ i       & 1]->getX(),
		                      lowMidPoints[(i >> 1) & 1]->getY(),
		                      lowMidPoints[(i >> 2) & 1]->getZ() );
		OctreeVector upperCorner( midHighPoints[ i       & 1]->getX(),
		                      midHighPoints[(i >> 1) & 1]->getY(),
		                      midHighPoints[(i >> 2) & 1]->getZ() );
		if (Max.x >= lowerCorner.getX() && Max.y >= lowerCorner.getY() && Max.z >= lowerCorner.getZ() &&
			Min.x <= upperCorner.getX() && Min.y <= upperCorner.getY() && Min.z <= upperCorner.getZ())
		{
			flags |= 1 << i;
		}
//		flags |= int(isOverlappingCell( item, lowerCorner, upperCorner )) << i;
	}

	return flags;
}

void COctreeVisitorElementNearLine::visitRoot(const OctreeCell* pRootCell, const OctreeData& octreeData)
{
	if (pRootCell != 0)
	{
		m_Used.clear();
		pRootCell->visit(octreeData, *this);
	}
}

void COctreeVisitorElementNearLine::visitBranch(const OctreeCell* subCells[8], const OctreeData& octreeData)
{
	const OctreeVector &LowerCorner = octreeData.getBound().getLowerCorner();
	const OctreeVector &UpperCorner = octreeData.getBound().getUpperCorner();
	const OctreeVector &Center = octreeData.getBound().getCenter();
	XYZ Bounds[3];
	Bounds[0] = XYZ(LowerCorner.getX(), LowerCorner.getY(), LowerCorner.getZ());	// Min
	Bounds[1] = XYZ(Center.getX(), Center.getY(), Center.getZ());	// Mid
	Bounds[2] = XYZ(UpperCorner.getX(), UpperCorner.getY(), UpperCorner.getZ());	// Max
	vector<bool> IntersectingCells;
	IntersectingCells.resize(8, false);
	XYZ P;
	double u;
	double dDenom;
	int i, j;
	int x, y, z;

	/////////
	// Find out if the line intersects any of the planes that build up the subcells
	/////////

	// i represents the plane direction (0 - x, 1 - y, 2 - z)
	for (i=0; i<3; ++i)
	{
		x = i;
		y = (i+1)%3;
		z = (i+2)%3;
		// j represents the plane distance (0 - min, 1 - mid, 2 - max)
		dDenom = m_P2[x] - m_P1[x];
		if (abs(dDenom) > 1e-6)
		{
			for (j=0; j<3; ++j)
			{
				u = (Bounds[j][x] - m_P1[x]) / dDenom;
				if (u < 0 && m_TrimLine.first)
					continue;
				if (u > 1 && m_TrimLine.second)
					continue;
				P = m_P1 + (m_P2 - m_P1) * u;
				if (Bounds[0][y] <= P[y] && P[y] <= Bounds[1][y])
				{
					if (Bounds[0][z] <= P[z] && P[z] <= Bounds[1][z])
					{
						if (j <= 2)
						{
							IntersectingCells[(0<<x) | (0<<y) | (0<<z)] = true;
						}
						if (j >= 0)
						{
							IntersectingCells[(1<<x) | (0<<y) | (0<<z)] = true;
						}
					}
					else if (Bounds[1][z] <= P[z] && P[z] <= Bounds[2][z])
					{
						if (j <= 2)
						{
							IntersectingCells[(0<<x) | (0<<y) | (1<<z)] = true;
						}
						if (j >= 0)
						{
							IntersectingCells[(1<<x) | (0<<y) | (1<<z)] = true;
						}
					}
				}
				else if (Bounds[1][y] <= P[y] && P[y] <= Bounds[2][y])
				{
					if (Bounds[0][z] <= P[z] && P[z] <= Bounds[1][z])
					{
						if (j <= 2)
						{
							IntersectingCells[(0<<x) | (1<<y) | (0<<z)] = true;
						}
						if (j >= 0)
						{
							IntersectingCells[(1<<x) | (1<<y) | (0<<z)] = true;
						}
					}
					else if (Bounds[1][z] <= P[z] && P[z] <= Bounds[2][z])
					{
						if (j <= 2)
						{
							IntersectingCells[(0<<x) | (1<<y) | (1<<z)] = true;
						}
						if (j >= 0)
						{
							IntersectingCells[(1<<x) | (1<<y) | (1<<z)] = true;
						}
					}
				}
			}
		}
	}

	/////////
	// Find out if one of the line ends is inside the cell if both m_TrimLine are set
	/////////

	if (m_TrimLine.first && m_TrimLine.second)
	{
		if (m_P1.x >= Bounds[0].x && m_P1.y >= Bounds[0].y && m_P1.z >= Bounds[0].z &&
			m_P1.x <= Bounds[2].x && m_P1.y <= Bounds[2].y && m_P1.z <= Bounds[2].z)
		{
			int dwCell = 0;
			if (m_P1.x > Bounds[1].x)
				dwCell |= 1 << 0;
			if (m_P1.y > Bounds[1].y)
				dwCell |= 1 << 1;
			if (m_P1.z > Bounds[1].z)
				dwCell |= 1 << 2;
			IntersectingCells[dwCell] = true;
		}
	}


	for (int i = 0; i < 8; ++i)
	{
		const OctreeCell* pSubCell = subCells[i];
		if(IntersectingCells[i] == true && pSubCell != 0)
		{
			const OctreeData subCellData(octreeData, i);
			pSubCell->visit(subCellData, *this);
		}
	}
}

void COctreeVisitorElementNearLine::visitLeaf(const OctreeArray<const MESH_ELEMENT*>& items, const OctreeData& octreeData)
{
	int i;
	int iNumItems = items.getLength();
	for (i=0; i<iNumItems; ++i)
	{
		const MESH_ELEMENT &Element = *items[i];
		if (Element.iIndex >= (int)m_Used.size())
		{
			m_Used.resize(Element.iIndex+1, false);
		}
		if (m_Used[Element.iIndex] == false)
		{
			m_Elements.push_back(Element.NodeIndices);
			m_Used[Element.iIndex] = true;
		}
	}
}




