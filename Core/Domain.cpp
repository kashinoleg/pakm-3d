#include "Domain.h"
#include "TexGen.h"
#include "Yarn.h"

using namespace TexGen;

CDomain::CDomain(TiXmlElement &Element)
{
	TiXmlElement* pMesh = Element.FirstChildElement("Mesh");
	if (pMesh)
	{
		m_Mesh = CMesh(*pMesh);
	}
}

void CDomain::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	Element.SetAttribute("type", GetType());
	if (OutputType == OUTPUT_FULL)
	{
		TiXmlElement Mesh("Mesh");
		m_Mesh.PopulateTiXmlElement(Mesh, OutputType);
		Element.InsertEndChild(Mesh);
	}
}

vector<pair<int, int> > CDomain::ConvertLimitsToInt(const vector<pair<double, double> > &RepeatLimits) {
	vector<pair<int, int> > IntRepeatLimits;
	for (pair<double, double> repeatLimit : RepeatLimits) {
		IntRepeatLimits.push_back(pair<int, int>((int)ceil(repeatLimit.first), (int)floor(repeatLimit.second)));
	}
	return IntRepeatLimits;
}

double CDomain::GetVolume() const {
	return m_Mesh.CalculateVolume();
}
