#pragma once
#include "SectionMesh.h"
#include "Logger.h"

/// Create a rectangular mesh, the number of layers can be specified or set as -1 for automatic determination
namespace TexGen {
	using namespace std;

	class CLASS_DECLSPEC CSectionMeshRectangular : public CSectionMesh {
	public:
		CSectionMeshRectangular(int iNumLayers = -1, bool bTriangleCorners = true);
		CSectionMeshRectangular(TiXmlElement &Element);
		~CSectionMeshRectangular(void);

		CSectionMesh* Copy() const { return new CSectionMeshRectangular(*this); }
		string GetType() const { return "CSectionMeshRectangular"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		void SetNumLayers( int iNum );
		int GetNumLayers() const {return m_iNumLayers;}
		int CalculateNumberofLayers(const vector<XY> &Section) const;
	protected:
		bool CreateMesh(const vector<XY> &Section) const;
		bool CreateSingleLayerMesh(const vector<XY> &Section) const;

		int m_iNumLayers;
		bool m_bTriangleCorners;
	};
}
