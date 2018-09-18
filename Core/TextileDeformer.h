#pragma once
#include "LinearTransformation.h"

namespace TexGen {
	using namespace std;

	class CTextile;

	/// A virtual base class which can be used to deform textiles
	/**
	Given a displacement field, this class will modify the geometry of an existing
	textile. Classes deriving from this class should override the GetDisplacement
	function to define the displacement field.
	*/
	class CLASS_DECLSPEC CTextileDeformer {
	public:
		CTextileDeformer(void);
		virtual ~CTextileDeformer(void);

		virtual void DeformTextile(CTextile &Textile, bool bDeformDomain = true);
		CTextile* GetDeformedCopyOfTextile(CTextile &Textile, bool bDeformDomain = true);

		const CLinearTransformation &GetRepeatVectorDeformation() const { return m_RepeatDeformation; }
		void SetRepeatVectorDeformation(CLinearTransformation RepeatDeformation) { m_RepeatDeformation =  RepeatDeformation; }

	protected:
		/**
		Get the displacement of a given point
		\param Pos The position of which the displacement is requested
		\param iYarn The yarn number for which the displacement is requested
					 this is necessary because sometimes yarns may overlap
					 and the yarns may have different displacements at the
					 same point in space
		\param Disp The displacement value should be filled in here by the function
		\return Should give an idea of how accurate the displacement value is,
				a value higher than 0 is considered to be very accurate if the
				value is less then it may represent an extrapolated estimate.
				This function may be called several times for the same point
				but offset by different repeat vectors and the displacement
				with highest value will be chosen.
		*/
		virtual double GetDisplacement(XYZ Pos, int iYarn, XYZ &Disp) const = 0;

		CLinearTransformation m_RepeatDeformation;
	};
}
