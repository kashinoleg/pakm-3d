#pragma once
#include "Section.h"

/// <summary>
/// Сечение эллипса мощности нити.
/// Эллипс мощности эквивалентен эллипсу, где по координате y прикладывается сила, до масштабирования до заданной высоты.
/// При мощности ниже 1 сечение напоминает прямоугольник с закругленными краями,
/// когда больше 1 сечение напоминает линзообразное сечение, а когда равно 1, то секция представляет собой эллипс.
/// Смещение по X перемещает максимальную высоту до заданного смещения от центра.
/// </summary>
namespace TexGen { 
	using namespace std;

	class CLASS_DECLSPEC CSectionPowerEllipse : public CSection {
	public:
		CSectionPowerEllipse(double dWidth, double dHeight, double dPower, double dXOffset=0.0);
		CSectionPowerEllipse(TiXmlElement &Element);
		~CSectionPowerEllipse(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionPowerEllipse(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionPowerEllipse"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;
		double GetTangent( double t) const;

		// Accessor methods
		double GetWidth() const { return m_dWidth; }
		double GetHeight() const { return m_dHeight; }
		double GetPower() const { return m_dPower; }
		double GetXOffset() const { return m_dXOffset; }
		void SetWidth( double dWidth ){ m_dWidth = dWidth; }
		void SetHeight( double dHeight ){ m_dHeight = dHeight; }
		void SetPower( double dPower ){ m_dPower = dPower; }

	protected:
		double m_dWidth;
		double m_dHeight;
		double m_dPower;
		double m_dXOffset;
	};
}
