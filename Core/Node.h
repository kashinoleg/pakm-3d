#pragma once
#include "Logger.h"

class TiXmlElement;
namespace TexGen {
	using namespace std;

	/// Represents a point on the centreline of a yarn
	class CLASS_DECLSPEC CNode {
	public:
		/// Create a node
		/**
		\param Position Position of the node
		\param Tangent Tangent of the node
		\param Up Up vector of the node
		*/
		CNode(XYZ Position = XYZ(), XYZ Tangent = XYZ(), XYZ Up = XYZ(0, 0, 1), double Angle = 0.0);
		CNode(TiXmlElement &Element);
		virtual ~CNode(void);

		/// Used for saving data to XML
		virtual void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Rotate the Node by given quaternion
		virtual void Rotate(WXYZ Rotation, XYZ Origin = XYZ(0,0,0));

		/// Translate the Node by given vector
		virtual void Translate(XYZ Vector);

		/// Project the Up vector such that it is perpendicular to the tangent
		void ProjectUp();

		/// Get the side vector
		//XYZ GetSide() const { return CrossProduct(m_Tangent, m_Up); }
		XYZ GetSide() const;
		//XYZ RotateSide( XYZ Side );
		// Various accessor methods
		XYZ GetPosition() const { return m_Position; }
		XYZ GetTangent() const { return m_Tangent; }
		XYZ GetUp() const { return m_Up; }
		double GetAngle() const { return m_Angle; }
		XYZ GetNormal() const;
		void SetPosition(XYZ Position) { m_Position = Position; }
		void SetTangent(XYZ Tangent) { m_Tangent = Tangent; }
		void SetUp(XYZ Up) { m_Up = Up; }
		void SetAngle( double Angle ) { m_Angle = Angle; }

	protected:
		/// Coordinates representing the position of the node
		XYZ m_Position;
		/// Unit vector tangent to the yarn path at this node
		XYZ m_Tangent;
		/// Unit vector which represents the up direction (must be perpendicular to m_Tangent)
		XYZ m_Up;
		double m_Angle;
	};
}
