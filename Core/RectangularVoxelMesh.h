#pragma once
#include "VoxelMesh.h"

namespace TexGen { 
	using namespace std;

	class CTextile;	

	/// Class used to generate voxel mesh for output to ABAQUS
	class CLASS_DECLSPEC CRectangularVoxelMesh : public CVoxelMesh {
	public:
		/// 
		/**
		\param Type String giving periodic boundary condition type 
		*/
		CRectangularVoxelMesh(string Type= "CPeriodicBoundaries");
		
		virtual ~CRectangularVoxelMesh(void);
	protected:
		/// Calculate voxel size based on number of voxels on each axis and domain size
		bool CalculateVoxelSizes(CTextile &Textile);
		
		/// Save voxel mesh in Abaqus .inp format with periodic boundary conditions
		/// bOutputMatrix and bOutput yarn specify which of these are saved to the Abaqus file
		void SaveToAbaqus( string Filename, CTextile &Textile, bool bOutputMatrix, bool bOutputYarn, int iBoundaryConditions, int iElementType );
		
		/// Outputs nodes to .inp file and gets element information
		void OutputNodes(ostream &Output, CTextile &Textile, bool bAbaqus = true );

		/// Voxel size for each axis
		double			m_VoxSize[3];
	};
}
