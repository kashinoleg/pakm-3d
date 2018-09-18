#pragma once
#include "Section.h"
#include "Logger.h"

namespace TexGen {
	using namespace std;

	/// Lenticular Section
	class CLASS_DECLSPEC CSectionLenticular : public CSection {
	public:
		CSectionLenticular(double dWidth, double dHeight, double dDistortion = 0);
		CSectionLenticular(TiXmlElement &Element);
		~CSectionLenticular(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionLenticular(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionLenticular"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;

		// Accessor methods
		double GetWidth() const { return m_dWidth; }
		double GetHeight() const { return m_dHeight; }
		double GetDistortion() const { return m_dDistortion; }

	protected:
		double m_dWidth;
		double m_dHeight;
		double m_dDistortion;
	};
}
