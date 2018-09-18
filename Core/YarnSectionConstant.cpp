#include "YarnSectionConstant.h"
#include "SectionEllipse.h"

using namespace TexGen;

CYarnSectionConstant::CYarnSectionConstant(const CSection &Section)
: m_pSection(Section)
{
	
}

CYarnSectionConstant::~CYarnSectionConstant(void)
{
}

CYarnSectionConstant::CYarnSectionConstant(TiXmlElement &Element)
: CYarnSection(Element)
{
	TiXmlElement* pSection = Element.FirstChildElement("Section");
	if (pSection)
		m_pSection = CSection::CreateSection(*pSection);
	else
		m_pSection = CSectionEllipse(1, 1);
}

void CYarnSectionConstant::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CYarnSection::PopulateTiXmlElement(Element, OutputType);

	TiXmlElement Section("Section");
	m_pSection->PopulateTiXmlElement(Section, OutputType);
	Element.InsertEndChild(Section);
}

vector<XY> CYarnSectionConstant::GetSection(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced) const
{
	// return the same section ignoring the information given in PositionInfo
	return m_pSection->GetPoints(iNumPoints, bEquiSpaced);
}

CMesh CYarnSectionConstant::GetSectionMesh(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced) const
{
	// return the same section ignoring the information given in PositionInfo
	return m_pSection->GetMesh(iNumPoints, bEquiSpaced);
}


