#include "SectionRotated.h"

using namespace TexGen;

CSectionRotated::CSectionRotated(const CSection &Section, double dAngle)
: m_dAngle(dAngle),m_pSection(Section) {
	if (m_pSection->GetType() == "CSectionRectangle") {
		m_pSectionMesh = CSectionMeshRectangleSection();
	}
}

CSectionRotated::~CSectionRotated(void) {
}

bool CSectionRotated::operator == (const CSection &CompareMe) const {
	if (CompareMe.GetType() != GetType()) {
		return false;
	}
	return m_dAngle == ((CSectionRotated*)&CompareMe)->m_dAngle &&
		*m_pSection == *((CSectionRotated*)&CompareMe)->m_pSection;
}

CSectionRotated::CSectionRotated(TiXmlElement &Element)
: CSection(Element) {
	Element.Attribute("Angle", &m_dAngle);
	TiXmlElement* pSection = Element.FirstChildElement("Section");
	if (pSection)
		m_pSection = CreateSection(*pSection);
}

void CSectionRotated::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const {
	CSection::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("Angle", stringify(m_dAngle));
	TiXmlElement Section("Section");
	m_pSection->PopulateTiXmlElement(Section, OutputType);
	Element.InsertEndChild(Section);
}

string CSectionRotated::GetDefaultName() const {
	return "Rotated(" + m_pSection->GetDefaultName() + ",R:" + stringify(m_dAngle) + ")";
}

string CSectionRotated::GetBaseType() const {
	return m_pSection->GetType();
}

XY CSectionRotated::GetPoint(double t) const {
	XY Point = m_pSection->GetPoint(t);
	XY RotatedPoint;
	RotatedPoint.x = Point.x*cos(m_dAngle) - Point.y*sin(m_dAngle);
	RotatedPoint.y = Point.x*sin(m_dAngle) + Point.y*cos(m_dAngle);
	return RotatedPoint;
}
