#pragma once
#include "Section.h"

/// <summary>
/// Прямоугольное сечение.
/// Создает прямоугольное сечение, где точки вокруг периметра расположены как можно равномерно вдоль сторон
/// в пределах ограничения наличия точки в каждом углу.
/// </summary>
namespace TexGen {
	using namespace std;

	class CLASS_DECLSPEC CSectionRectangle : public CSection {
	public:
		CSectionRectangle(double dWidth, double dHeight);
		CSectionRectangle(TiXmlElement &Element);
		~CSectionRectangle(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionRectangle(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Get a section with given number of points on the perimeter
		virtual const vector<XY> &GetPoints(int iNumPoints, bool bEquiSpaced = false) const;

		string GetType() const { return "CSectionRectangle"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;

		// Accessor methods
		double GetWidth() const { return m_dWidth; }
		double GetHeight() const { return m_dHeight; }
		void SetWidth( double dWidth ){ m_dWidth = dWidth; }
		void SetHeight( double dHeight ){ m_dHeight = dHeight; }

	protected:
		double m_dWidth;
		double m_dHeight;
		double m_XSpacing;
		double m_YSpacing;
	};
}
