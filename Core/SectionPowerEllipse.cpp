#include "SectionPowerEllipse.h"

using namespace TexGen;

CSectionPowerEllipse::CSectionPowerEllipse(double dWidth, double dHeight, double dPower, double dXOffset)
: m_dWidth(dWidth)
, m_dHeight(dHeight)
, m_dPower(dPower)
, m_dXOffset(dXOffset)
{
}

CSectionPowerEllipse::~CSectionPowerEllipse(void) {
}

bool CSectionPowerEllipse::operator == (const CSection &CompareMe) const
{
	if (CompareMe.GetType() != GetType())
		return false;
	return m_dWidth == ((CSectionPowerEllipse*)&CompareMe)->m_dWidth &&
		m_dHeight == ((CSectionPowerEllipse*)&CompareMe)->m_dHeight &&
		m_dPower == ((CSectionPowerEllipse*)&CompareMe)->m_dPower &&
		m_dXOffset == ((CSectionPowerEllipse*)&CompareMe)->m_dXOffset;
}

CSectionPowerEllipse::CSectionPowerEllipse(TiXmlElement &Element)
: CSection(Element)
{
	Element.Attribute("Width", &m_dWidth);
	Element.Attribute("Height", &m_dHeight);
	Element.Attribute("Power", &m_dPower);
	Element.Attribute("XOffset", &m_dXOffset);
}

void CSectionPowerEllipse::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CSection::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("Width", stringify(m_dWidth));
	Element.SetAttribute("Height", stringify(m_dHeight));
	Element.SetAttribute("Power", stringify(m_dPower));
	Element.SetAttribute("XOffset", stringify(m_dXOffset));
}

XY CSectionPowerEllipse::GetPoint(double t) const
{	
	double dX, dY;
	if ( t <= 0.25 || t >= 0.75 )
		dX = (0.5*m_dWidth - m_dXOffset)*cos(t*2*PI) + m_dXOffset;
	else
		dX = (0.5*m_dWidth + m_dXOffset)*cos(t*2*PI) + m_dXOffset;

	if ( t <= 0.5 )
		dY = 0.5*m_dHeight*pow(sin(t*2*PI), m_dPower);
	else
		dY = -0.5*m_dHeight*pow(-sin(t*2*PI), m_dPower);

	return XY( dX, dY );
}

string CSectionPowerEllipse::GetDefaultName() const
{
	return "PowerEllipse(W:" + stringify(m_dWidth) + ",H:" + stringify(m_dHeight) + ",P:" + stringify(m_dPower) + ")";
}

double CSectionPowerEllipse::GetTangent( double t ) const
{
	if ( t <= 0.25 )
	{
		return( (m_dHeight * m_dPower * cos(t*2*PI) * pow(sin(t*2*PI), m_dPower-2)) / ( -2.0 * (0.5*m_dWidth - m_dXOffset)) );
	}
	else if ( t > 0.25 && t <= 0.5 )
	{
		return( (m_dHeight * m_dPower * cos(t*2*PI) * pow(sin(t*2*PI), m_dPower-2)) / ( -2.0 * (0.5*m_dWidth + m_dXOffset)) );
	}
	else if ( t > 0.5 && t < 0.75 )
	{
		return( (m_dHeight * m_dPower * cos(t*2*PI) * pow(-sin(t*2*PI), m_dPower-1)) / ( -2.0 * (0.5*m_dWidth + m_dXOffset) * sin(t*2*PI)) );
	}
	else
	{
		return( (m_dHeight * m_dPower * cos(t*2*PI) * pow(-sin(t*2*PI), m_dPower-1)) / ( -2.0 * (0.5*m_dWidth - m_dXOffset) * sin(t*2*PI)) );
	}
}
