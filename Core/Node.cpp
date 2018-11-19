#include "stdafx.h"
#include "Node.h"

using namespace TexGen;

CNode::CNode(XYZ Position, XYZ Tangent, XYZ Up, double Angle)
: m_Position(Position)
, m_Tangent(Tangent)
, m_Up(Up)
, m_Angle(Angle)
{
	double dLength;
	dLength = GetLength(m_Tangent);
	if (dLength != 0)
		m_Tangent /= dLength;
	dLength = GetLength(m_Up);
	if (dLength != 0)
		m_Up /= dLength;
}

CNode::~CNode(void)
{
}

CNode::CNode(TiXmlElement &Element)
{
	m_Position = valueify<XYZ>(Element.Attribute("Position"));
	m_Tangent = valueify<XYZ>(Element.Attribute("Tangent"));
	m_Up = valueify<XYZ>(Element.Attribute("Up"));
	m_Angle = valueify<double>(Element.Attribute("Angle"));
}

void CNode::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	Element.SetAttribute("Position", stringify(GetPosition()));
	Element.SetAttribute("Tangent", stringify(GetTangent()));
	Element.SetAttribute("Up", stringify(GetUp()));
	Element.SetAttribute("Angle", stringify(GetAngle()));
}

void CNode::Rotate(WXYZ Rotation, XYZ Origin)
{
	m_Position = Rotation * (m_Position-Origin) + Origin;
	m_Tangent = Rotation * m_Tangent;
	m_Up = Rotation * m_Up;
}

void CNode::Translate(XYZ Vector)
{
	m_Position += Vector;
}

void CNode::ProjectUp()
{
	m_Up -= m_Tangent * DotProduct(m_Tangent, m_Up);
	if (GetLength(m_Up) == 0)
	{
		TGERROR("Error projecting up vector to be orthogonal to the tangent vector");
		assert(false);
	}
	else
		Normalise(m_Up);
}


XYZ CNode::GetSide() const
{
	XYZ Side = CrossProduct(m_Tangent, m_Up);
	if ( m_Angle == 0.0 )
		return Side;
	WXYZ Rot( m_Up, m_Angle );
	XYZ RotatedSide = Rot*Side;
	return RotatedSide;
}

XYZ CNode::GetNormal() const
{
	if ( m_Angle == 0.0 )
		return m_Tangent;
	WXYZ Rot( m_Up, m_Angle );
	return Rot*m_Tangent;
}

/*XYZ CNode::RotateSide( XYZ Side )
{
	WXYZ Rot( m_Up, PI/9.0 );
	XYZ RotatedSide = Rot*Side;
	return RotatedSide;
}*/
