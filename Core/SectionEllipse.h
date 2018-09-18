#pragma once
#include "Section.h"

namespace TexGen { 
	using namespace std;

	/// Elliptical Section
	class CLASS_DECLSPEC CSectionEllipse : public CSection {
	public:
		CSectionEllipse(double dWidth, double dHeight);
		CSectionEllipse(TiXmlElement &Element);
		~CSectionEllipse(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionEllipse(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionEllipse"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;

		// Accessor methods
		double GetWidth() const { return m_dWidth; }
		double GetHeight() const { return m_dHeight; }
		void SetWidth( double dWidth ){ m_dWidth = dWidth; }

	protected:
		double m_dWidth;
		double m_dHeight;
	};
}
