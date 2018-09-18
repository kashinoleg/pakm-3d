#pragma once
#include "Section.h"

/// <summary>
/// Создает полигональное сечение, где список заданных точек, образует замкнутый полигон.
/// Точки находятся в локальных координатах XY относительно центральной линии нити.
/// Точки начинаются с(maxX, 0) и упорядочиваются в направлении против часовой стрелки
/// </summary>
namespace TexGen {
	using namespace std;

	class CLASS_DECLSPEC CSectionPolygon : public CSection {
	public:
		CSectionPolygon(const vector<XY> &PolygonPoints, bool bSingleQuadrant = false);
		CSectionPolygon(TiXmlElement &Element);
		~CSectionPolygon(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionPolygon(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionPolygon"; }
		string GetDefaultName() const;

		XY GetPoint(double t) const;

		/// Change the scale of the section by multiplying each coordinate component by the component given by this XY struct
		void Scale(XY Scale);

		/// Change the scale of the section by multiplying each coordinate value by a scalar
		void Scale(double dScale);

	protected:
		/// Assign t value as proportion of distance around perimeter for each point
		void CalcTValues();

		vector<XY> m_PolygonPoints;
		/// The proportion of the distance around the total perimeter range from 0 to 1 for each point
		vector<double> m_t;
	};
}
