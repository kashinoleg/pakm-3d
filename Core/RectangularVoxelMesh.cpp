#include "RectangularVoxelMesh.h"
#include "TexGen.h"
#include "PeriodicBoundaries.h"
#include <iterator>

using namespace TexGen;

CRectangularVoxelMesh::CRectangularVoxelMesh(string Type)
:CVoxelMesh(Type) {
}

CRectangularVoxelMesh::~CRectangularVoxelMesh(void) {
}

bool CRectangularVoxelMesh::CalculateVoxelSizes(CTextile &Textile) {
	XYZ DomSize;
	m_DomainAABB = Textile.GetDomain()->GetMesh().GetAABB();
	DomSize = m_DomainAABB.second - m_DomainAABB.first;
	m_VoxSize[0] = DomSize.x / m_XVoxels;
	m_VoxSize[1] = DomSize.y / m_YVoxels;
	m_VoxSize[2] = DomSize.z / m_ZVoxels;
	return true;
}

void CRectangularVoxelMesh::OutputNodes(ostream &Output, CTextile &Textile, bool bAbaqus )
{
	int iNodeIndex = 1;
	vector<XYZ> CentrePoints;
	vector<POINT_INFO> RowInfo;

	if ( !bAbaqus )  // if outputting in SCIRun format need to output number of voxels
		Output << (m_XVoxels+1)*(m_YVoxels+1)*(m_ZVoxels+1) << endl;
	
	for (int  z = 0; z <= m_ZVoxels; ++z )
	{
		for (int y = 0; y <= m_YVoxels; ++y )
		{
			for (int x = 0; x <=m_XVoxels; ++x )
			{
				XYZ Point;
				Point.x = m_DomainAABB.first.x + m_VoxSize[0] * x;
				Point.y = m_DomainAABB.first.y + m_VoxSize[1] * y;
				Point.z = m_DomainAABB.first.z + m_VoxSize[2] * z;
				if ( bAbaqus )
					Output << iNodeIndex << ", ";
				Output << Point << endl;

				if ( x < m_XVoxels && y < m_YVoxels && z < m_ZVoxels )
				{
					Point.x += 0.5*m_VoxSize[0];
					Point.y += 0.5*m_VoxSize[1];
					Point.z += 0.5*m_VoxSize[2];
					CentrePoints.push_back(Point);
				}
				++iNodeIndex;
			}
			
		}
		RowInfo.clear();   // Changed to do layer at a time instead of row to optimise
			Textile.GetPointInformation( CentrePoints, RowInfo );
			m_ElementsInfo.insert(m_ElementsInfo.end(), RowInfo.begin(), RowInfo.end() );
			CentrePoints.clear();
	}
	//Textile.GetPointInformation( CentrePoints, m_ElementsInfo );
}
