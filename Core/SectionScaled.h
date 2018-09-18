#pragma once
#include "Section.h"

namespace TexGen { 
	using namespace std;

	/// Section which represents a scaled version of another section
	class CLASS_DECLSPEC CSectionScaled : public CSection {
	public:
		CSectionScaled(const CSection &Section, XY Scale);
		CSectionScaled(TiXmlElement &Element);
		~CSectionScaled(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionScaled(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionScaled"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;

		// Accessor methods
		const CSection &GetSection() const { return *m_pSection; }
		XY GetScale() const { return m_Scale; }

	protected:
		XY m_Scale;
		CObjectContainer<CSection> m_pSection;
	};
}
