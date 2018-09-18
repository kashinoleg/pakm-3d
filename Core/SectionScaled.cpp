#include "SectionScaled.h"

using namespace TexGen;

CSectionScaled::CSectionScaled(const CSection &Section, XY Scale)
:  m_Scale(Scale), m_pSection(Section) {
}

CSectionScaled::~CSectionScaled(void) {
}

bool CSectionScaled::operator == (const CSection &CompareMe) const {
	if (CompareMe.GetType() != GetType()) {
		return false;
	}
	else {
		return m_Scale == ((CSectionScaled*)&CompareMe)->m_Scale &&
			*m_pSection == *((CSectionScaled*)&CompareMe)->m_pSection;
	}
}

CSectionScaled::CSectionScaled(TiXmlElement &Element)
: CSection(Element)
{
	m_Scale = valueify<XY>(Element.Attribute("Scale"));
	TiXmlElement* pSection = Element.FirstChildElement("Section");
	if (pSection)
		m_pSection = CreateSection(*pSection);
}

void CSectionScaled::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CSection::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("Scale", stringify(m_Scale));
	TiXmlElement Section("Section");
	m_pSection->PopulateTiXmlElement(Section, OutputType);
	Element.InsertEndChild(Section);
}


string CSectionScaled::GetDefaultName() const
{
	return "Scaled(" + m_pSection->GetDefaultName() + ",S:" + stringify(m_Scale) + ")";
}

XY CSectionScaled::GetPoint(double t) const
{
	XY Point = m_pSection->GetPoint(t);
	XY ScalePoint;
	ScalePoint.x = Point.x * m_Scale.x;
	ScalePoint.y = Point.y * m_Scale.y;
	return ScalePoint;
}
