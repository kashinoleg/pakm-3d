#pragma once
#include "Logger.h"

namespace TexGen {
	using namespace std;

	class CLASS_DECLSPEC CProperty {
	public:
		CProperty(string SIUnits);

		void WriteAttribute(TiXmlElement &Element, string AttributeName);
		void ReadAttribute(TiXmlElement &Element, string AttributeName);

		string GetString() const;
		double GetValue(string Units) const;
		double GetSIValue() const;
		void SetValue(double dValue, string Units);
		void SetValue( double dValue );
		void SetUnits( string Units );
		void Reset();

		double GetValue() const;
		string GetUnits() const;
		string GetSIUnits() const;
		bool IsSet() const;

	protected:
		double m_dValue;
		string m_Units;
		string m_SIUnits;
		bool m_bSet;
	};
}
