#pragma once
#include "TextileDeformer.h"
#include "Mesh.h"

namespace TexGen {
	using namespace std;

	class CTextile;

	/// Deform the geometry of a textile given a volume mesh
	/**
	The idea is that a volume mesh of a textile can be created and used to run
	a mechanical analysis using a finite element analysis package. The displacements
	of the nodes calculated by the FE package are read back in with this class. And
	using these displacements, the textile geometry is deformed.
	*/
	class CLASS_DECLSPEC CTextileDeformerVolumeMesh : public CTextileDeformer {
	public:
		CTextileDeformerVolumeMesh(void);
		virtual ~CTextileDeformerVolumeMesh(void);

		bool SetYarnMeshDisplacements(int iYarn, const CMesh &Mesh, vector<XYZ> &Displacements);

	protected:
		struct YARN_MESH
		{
			CMesh Mesh;
			vector<XYZ> NodeDisplacements;
		};

		virtual double GetDisplacement(XYZ Pos, int iYarn, XYZ &Disp) const;
		void GetBarycentricCoordinates(const XYZ &P, const XYZ &P1, const XYZ &P2, const XYZ &P3, const XYZ &P4, double &a, double &b, double &c, double &d) const;

		vector<YARN_MESH> m_YarnMeshes;
	};
}
