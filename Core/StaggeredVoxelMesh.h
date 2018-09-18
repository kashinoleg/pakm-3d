#pragma once
#include "RectangularVoxelMesh.h"

namespace TexGen { 
	using namespace std;

	class CTextile;	

	/// Class used to generate voxel mesh for output to ABAQUS where a reduced unit cell is obtained by use of a staggered offset repeat
	class CLASS_DECLSPEC CStaggeredVoxelMesh : public CRectangularVoxelMesh {
	public:
		CStaggeredVoxelMesh(string Type = "CStaggeredPeriodicBoundaries");
		virtual ~CStaggeredVoxelMesh(void);

		void SetOffset( double Offset );

	protected:
		/// Calculate voxel size based on number of voxels on each axis and domain size
		bool CalculateVoxelSizes(CTextile &Textile);

		void OutputPeriodicBoundaries(ostream &Output, CTextile& Textile, int iBoundaryConditions, bool bMatrixOnly );
		double m_Offset;
	};
}
