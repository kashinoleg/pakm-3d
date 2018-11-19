#include "stdafx.h"
#include "Property.h"

using namespace TexGen;

CProperty::CProperty(string SIUnits)
: m_SIUnits(SIUnits)
, m_dValue(0)
, m_bSet(false) {
}

void CProperty::WriteAttribute(TiXmlElement &Element, string AttributeName) {
	if (m_bSet) {
		Element.SetAttribute(AttributeName+"Value", stringify(m_dValue));
		Element.SetAttribute(AttributeName+"Units", m_Units);
	}
}

void CProperty::ReadAttribute(TiXmlElement &Element, string AttributeName) {
	double dValue = 0;
	const string* pUnits;
	if (Element.Attribute(AttributeName + "Value", &dValue))
	{
		pUnits = Element.Attribute(AttributeName + "Units");
		if (pUnits)
		{
			SetValue(dValue, *pUnits);
		}
	}
}

string CProperty::GetString() const {
	return stringify(m_dValue) + " " + m_Units;
}

double CProperty::GetValue(string Units) const {
	return ConvertUnits(m_dValue, m_Units, Units);
}

double CProperty::GetSIValue() const {
	return GetValue(m_SIUnits);
}

void CProperty::SetValue(double dValue, string Units) {
	string Error;
	if (CompatibleUnits(Units, m_SIUnits, &Error)) {
		m_dValue = dValue;
		m_Units = Units;
		m_bSet = true;
	}
	else {
		Reset();
		TGERROR("Error setting property: " << Error);
	}
}

void CProperty::SetValue( double dValue ) {
	m_dValue = dValue;
	m_bSet = true;
}

void CProperty::SetUnits( string Units ) {
	string Error;
	if ( CompatibleUnits( Units, m_SIUnits, &Error ) ) {
		m_Units = Units;
	}
	else {
		Reset();
		TGERROR("Error setting property: " << Error);
	}
}

void CProperty::Reset() {
	m_dValue = 0;
	m_Units.clear();
	m_bSet = false;
}

double CProperty::GetValue() const {
	return m_dValue;
}

string CProperty::GetUnits() const {
	return m_Units;
}

string CProperty::GetSIUnits() const {
	return m_SIUnits;
}

bool CProperty::IsSet() const {
	return m_bSet;
}
