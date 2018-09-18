#include "SectionMesh.h"
#include "SectionMeshTriangulate.h"
#include "SectionMeshRectangular.h"
#include "SectionMeshRectangleSection.h"

using namespace TexGen;

CSectionMesh::CSectionMesh(void) {
}

CSectionMesh::~CSectionMesh(void) {
}

CSectionMesh::CSectionMesh(TiXmlElement &Element) {
}

void CSectionMesh::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const {
	Element.SetAttribute("type", GetType());
}

CObjectContainer<CSectionMesh> CSectionMesh::CreateSectionMesh(TiXmlElement &Element) {
	const string* pType = Element.Attribute(string("type"));
	if (pType) {
		if (*pType == "CSectionMeshTriangulate")
			return CSectionMeshTriangulate(Element);
		else if (*pType == "CSectionMeshRectangular")
			return CSectionMeshRectangular(Element);
		else if (*pType == "CSectionMeshRectangleSection")
			return CSectionMeshRectangleSection(Element);
	}
	return CObjectContainer<CSectionMesh>();
}

bool CSectionMesh::CreateMeshIfNeeded(const vector<XY> &Section) const {
	if (Section != m_Section) {
		m_Section = Section;
		return CreateMesh(Section);
	}
	return true;
}

const CMesh &CSectionMesh::GetMesh(const vector<XY> &Section) const {
	CreateMeshIfNeeded(Section);
	return m_Mesh;
}
