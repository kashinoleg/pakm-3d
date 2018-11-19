#include "stdafx.h"
#include "SectionEllipse.h"

using namespace TexGen;

CSectionEllipse::CSectionEllipse(double dWidth, double dHeight)
: m_dWidth(dWidth)
, m_dHeight(dHeight)
{
}

CSectionEllipse::~CSectionEllipse(void) {
}

bool CSectionEllipse::operator == (const CSection &CompareMe) const {
	if (CompareMe.GetType() != GetType()) {
		return false;
	}
	return m_dWidth == ((CSectionEllipse*)&CompareMe)->m_dWidth &&
		m_dHeight == ((CSectionEllipse*)&CompareMe)->m_dHeight;
}

CSectionEllipse::CSectionEllipse(TiXmlElement &Element)
: CSection(Element)
, m_dWidth(0)
, m_dHeight(0) {
	Element.Attribute("Width", &m_dWidth);
	Element.Attribute("Height", &m_dHeight);
}

void CSectionEllipse::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const {
	CSection::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("Width", stringify(m_dWidth));
	Element.SetAttribute("Height", stringify(m_dHeight));
}

XY CSectionEllipse::GetPoint(double t) const {
	return XY(0.5*m_dWidth*cos(t*2*PI), 0.5*m_dHeight*sin(t*2*PI));
}

string CSectionEllipse::GetDefaultName() const {
	return "Ellipse(W:" + stringify(m_dWidth) + ",H:" + stringify(m_dHeight) + ")";
}
