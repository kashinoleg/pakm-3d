#include "stdafx.h"
#include "PropertiesTextile.h"
#include "DefaultProperties.h"

using namespace TexGen;

CPropertiesTextile::CPropertiesTextile()
: m_ArealDensity("kg/m^2")
,m_GeometryScale("mm")
, m_MatrixYoungsModulus("MPa")
, m_MatrixAlpha(MATRIX_ALPHA)
, m_MatrixPoissonsRatio(0.1)
{
	SetDefaultMatrixProperties();
}

CPropertiesTextile::CPropertiesTextile(TiXmlElement &Element)
: CProperties(Element), m_ArealDensity("kg/m^2"),  m_GeometryScale("mm")
, m_MatrixYoungsModulus("MPa")
, m_MatrixAlpha(MATRIX_ALPHA)
, m_MatrixPoissonsRatio(0.1)
{
	const string* pGeomScale = Element.Attribute(string("GeometryScale"));
	if (pGeomScale)
		m_GeometryScale = *pGeomScale;
	m_ArealDensity.WriteAttribute(Element, "ArealDensity");
	m_MatrixYoungsModulus.ReadAttribute(Element, "MatrixYoungsModulus");
	Element.Attribute("MatrixPoissonsRatio", &m_MatrixPoissonsRatio);
	Element.Attribute("MatrixAlpha", &m_MatrixAlpha);
}

CPropertiesTextile::~CPropertiesTextile(void) {
}

void CPropertiesTextile::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) {
	CProperties::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("GeometryScale", m_GeometryScale);
	m_ArealDensity.ReadAttribute(Element, "ArealDensity");
	m_MatrixYoungsModulus.WriteAttribute(Element, "MatrixYoungsModulus");
	if (m_MatrixPoissonsRatio != 0.0) {
		Element.SetAttribute("MatrixPoissonsRatio", stringify(m_MatrixPoissonsRatio));
	}
	if (m_MatrixAlpha != 0.0) {
		Element.SetAttribute("MatrixAlpha", stringify(m_MatrixAlpha));
	}
}

bool CPropertiesTextile::SetGeometryScale(string Units) {
	// Check the units are of the same type
	if (CompatibleUnits("m", Units)) {
		m_GeometryScale = Units;
		return true;
	}
	else {
		TGERROR("Unable to set geometry scale, units must represent a length!");
		return false;
	}
}

void CPropertiesTextile::SetArealDensity(double dValue, string Units) {
	m_ArealDensity.SetValue(dValue, Units);
}

string CPropertiesTextile::GetGeometryScale() const {
	return m_GeometryScale;
}

double CPropertiesTextile::GetArealDensity(string Units) const {
	return m_ArealDensity.GetValue(Units);
}

void CPropertiesTextile::SetMatrixYoungsModulus(double dValue, string Units) {
	m_MatrixYoungsModulus.SetValue(dValue, Units);
}

void CPropertiesTextile::SetMatrixPoissonsRatio(double dValue) {
	m_MatrixPoissonsRatio = dValue;
}

void CPropertiesTextile::SetMatrixAlpha(double dValue) {
	m_MatrixAlpha = dValue;
}

double CPropertiesTextile::GetMatrixYoungsModulus(string Units) const {
	return m_MatrixYoungsModulus.GetValue(Units);
}

double CPropertiesTextile::GetMatrixPoissonsRatio() const {
	return m_MatrixPoissonsRatio;
}

double CPropertiesTextile::GetMatrixAlpha() const {
	return m_MatrixAlpha;
}

void CPropertiesTextile::SetDefaultMatrixProperties() {
	SetMatrixYoungsModulus( MATRIX_E );	
	SetMatrixPoissonsRatio( MATRIX_POISSON );
	SetMatrixAlpha( MATRIX_ALPHA );
}
