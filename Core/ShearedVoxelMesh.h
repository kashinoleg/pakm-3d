#pragma once
#include "VoxelMesh.h"

namespace TexGen { 
	using namespace std;

	class CTextile;	

	/// Class used to generate voxel mesh for output to ABAQUS
	class CLASS_DECLSPEC CShearedVoxelMesh : public CVoxelMesh {
	public:
		CShearedVoxelMesh(string Type = "CShearedPeriodicBoundaries");
		virtual ~CShearedVoxelMesh(void);

	protected:
		/// Calculate voxel size based on number of voxels on each axis and domain size
		bool CalculateVoxelSizes(CTextile &Textile);
		
		/// Outputs nodes to .inp file and gets element information
		void OutputNodes(ostream &Output, CTextile &Textile, bool bAbaqus = true );

		XY m_ShearedVoxSize[3];
		XYZ m_StartPoint;
	};
}
