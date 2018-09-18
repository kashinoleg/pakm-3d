#pragma once
#include "SectionMesh.h"
#include "Logger.h"

namespace TexGen {
	using namespace std;

	/// Create a rectangular mesh, the number of layers can be specified or set as -1 for automatic determination
	/**
	Mesh is created with regular rectangular grid
	*/
	class CLASS_DECLSPEC CSectionMeshRectangleSection : public CSectionMesh {
	public:
		CSectionMeshRectangleSection(int iNumLayers = -1);
		CSectionMeshRectangleSection(TiXmlElement &Element);
		~CSectionMeshRectangleSection(void);

		CSectionMesh* Copy() const { return new CSectionMeshRectangleSection(*this); }
		string GetType() const { return "CSectionMeshRectangleSection"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		void SetNumLayers( int iNum );
		int GetNumLayers() const { return m_iNumLayers; }
		int CalculateNumberofLayers(const vector<XY> &Section) const;

	protected:
		bool CreateMesh(const vector<XY> &Section) const;

		int m_iNumLayers;
	};
}
