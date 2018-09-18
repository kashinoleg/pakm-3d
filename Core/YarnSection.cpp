#include "YarnSectionConstant.h"
#include "YarnSectionInterpNode.h"
#include "YarnSectionInterpPosition.h"
#include "YarnSectionAdjusted.h"
#include "SectionEllipse.h"

using namespace TexGen;

CYarnSection::CYarnSection(bool bConstMesh)
: m_bForceEqualMeshLayers(bConstMesh)
{
}

CYarnSection::~CYarnSection(void)
{
}

CYarnSection::CYarnSection(TiXmlElement &Element)
{
	const char* result = Element.Attribute("ConstMesh"); // Need to check whether attribute present so can set to true if not
	if (result != NULL )
	{
		m_bForceEqualMeshLayers = valueify<bool>(result);
	}
	else
		m_bForceEqualMeshLayers = true;
}

CObjectContainer<CYarnSection> CYarnSection::CreateYarnSection(TiXmlElement &Element)
{
	const string* pType = Element.Attribute(string("type"));
	if (pType)
	{
		if (*pType == "CYarnSectionConstant")
			return CYarnSectionConstant(Element);
		else if (*pType == "CYarnSectionInterpNode")
			return CYarnSectionInterpNode(Element);
		else if (*pType == "CYarnSectionInterpPosition")
			return CYarnSectionInterpPosition(Element);
		else if (*pType == "CYarnSectionAdjusted")
			return CYarnSectionAdjusted(Element);
	}
	// If the yarn section was not recognised for some reason, revert to using a constant
	// circular yarn section of diameter 1
	return CYarnSectionConstant(CSectionEllipse(1, 1));
}

void CYarnSection::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	Element.SetAttribute("type", GetType());
	Element.SetAttribute("ConstMesh", stringify(m_bForceEqualMeshLayers));
}

double YARN_POSITION_INFORMATION::GetYarnPosition() const
{
	double dYarnPosition;

	if (iSection < 0 || iSection >= (int)SectionLengths.size())
	{
		TGERROR("Unable to get yarn position, yarn section index out of range: " << iSection);
		return 0;
	}

	dYarnPosition = accumulate(SectionLengths.begin(), SectionLengths.begin()+iSection, 0.0);
	dYarnPosition += SectionLengths[iSection] * dSectionPosition;
	dYarnPosition /= accumulate(SectionLengths.begin(), SectionLengths.end(), 0.0);

	return dYarnPosition;
}

CYarnSectionConstant* CYarnSection::GetSectionConstant()
{
	return dynamic_cast<CYarnSectionConstant*>(this);
}







