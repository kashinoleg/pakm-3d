#pragma once
#include "Interpolation.h"

namespace TexGen {
	using namespace std;

	/// Bezier interpolation for yarn paths
	class CLASS_DECLSPEC CInterpolationLinear : public CInterpolation {
	public:
		CInterpolationLinear(bool bPeriodic = true, bool bForceInPlaneTangent = false, bool bForceMasterNodeTangent = false);
		CInterpolationLinear(TiXmlElement &Element);
		~CInterpolationLinear(void);

		CInterpolation* Copy() const { return new CInterpolationLinear(*this); }
		string GetType() const { return "CInterpolationLinear"; }
//		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Calculate the node tangents (use node tangents of they exist) and store them in m_Tangents
		void Initialise(const vector<CNode> &MasterNodes) const;

		/// Get a node from parametric function where t is specified
		CSlaveNode GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const;

	protected:
		mutable vector<XYZ> m_Tangents;
	};
}
