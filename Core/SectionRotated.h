#pragma once
#include "Section.h"

/// <summary>
/// Сечение, которое представляет поворот другого сечения на угол заданный в радианах
/// </summary>
namespace TexGen {
	using namespace std;

	class CLASS_DECLSPEC CSectionRotated : public CSection {
	public:
		/// The angle is given in radians as an anti-clockwise rotation, -ve angles represent a clockwise rotation
		CSectionRotated(const CSection &Section, double dAngle);
		CSectionRotated(TiXmlElement &Element);
		~CSectionRotated(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionRotated(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionRotated"; }
		string GetDefaultName() const;
		string GetBaseType() const;

		XY GetPoint(double t) const;

		// Accessor methods
		const CSection &GetSection() const { return *m_pSection; }
		double GetAngle() const { return m_dAngle; }

	protected:
		double m_dAngle;
		CObjectContainer<CSection> m_pSection;
	};
}
