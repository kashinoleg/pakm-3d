#pragma once
#include "Interpolation.h"

namespace TexGen {
	using namespace std;

	/// Bezier interpolation for yarn paths
	class CLASS_DECLSPEC CInterpolationBezier : public CInterpolation {
	public:
		CInterpolationBezier(bool bPeriodic = true, bool bForceInPlaneTangent = false, bool bForceMasterNodeTangent = false);
		CInterpolationBezier(TiXmlElement &Element);
		~CInterpolationBezier(void);

		CInterpolation* Copy() const { return new CInterpolationBezier(*this); }
		string GetType() const { return "CInterpolationBezier"; }
//		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Calculate the node tangents (use node tangents of they exist) and store them in m_Tangents
		void Initialise(const vector<CNode> &MasterNodes) const;

		/// Get a node from parametric function where t is specified
		CSlaveNode GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const;

	protected:
		mutable vector<XYZ> m_Tangents;
	};
}
