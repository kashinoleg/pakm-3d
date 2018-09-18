#pragma once
#include "Interpolation.h"

namespace TexGen {
	using namespace std;

	/// Cubic spline interpolation for yarn paths
	class CLASS_DECLSPEC CInterpolationCubic :	public CInterpolation {
	public:
		CInterpolationCubic(bool bPeriodic = true, bool bForceInPlaneTangent = false, bool bForceMasterNodeTangent = false);
		CInterpolationCubic(TiXmlElement &Element);
		~CInterpolationCubic(void);

		CInterpolation* Copy() const { return new CInterpolationCubic(*this); }
		string GetType() const { return "CInterpolationCubic"; }
//		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Create the spline cubic equations and store them in m_?Cubics
		void Initialise(const vector<CNode> &MasterNodes) const;

		/// Get a node from parametric function where t is specified
		CSlaveNode GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const;

	protected:
		/// Struct to represent a cubic equation
		struct CUBICEQUATION
		{
			CUBICEQUATION(double a, double b, double c, double d)
			: m_a(a)
			, m_b(b)
			, m_c(c)
			, m_d(d)
			{}

			double Evaluate(double x) const
			{
				return (((m_d*x)+m_c)*x+m_b)*x+m_a;
			}

			double EvaluateDerivative(double x) const
			{
				return ((3*m_d*x)+2*m_c)*x+m_b;
			}

		protected:
			double m_a;
			double m_b;
			double m_c;
			double m_d;
		};

		static void GetPeriodicCubicSplines(const vector<double> &Knots, vector<CUBICEQUATION> &Cubics);
		static void GetNaturalCubicSplines(const vector<double> &Knots, vector<CUBICEQUATION> &Cubics);

		mutable vector<CUBICEQUATION> m_XCubics;
		mutable vector<CUBICEQUATION> m_YCubics;
		mutable vector<CUBICEQUATION> m_ZCubics;
	};
}
