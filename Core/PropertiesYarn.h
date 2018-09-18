#pragma once
#include "Properties.h"

namespace TexGen {
	using namespace std;

	/// Class to store properties related to a textile
	/**
	This class combines geometrical information stored in the CTextile class with user
	input data relating to physical properties of the textile. Several interesting
	values can be obtained in this manner, for example the volume fraction is calculated
	from the volume of yarns in the geometrical model along with information about fibre
	density and areal density.
	*/
	class CLASS_DECLSPEC CPropertiesYarn : public CProperties {
	public:
		CPropertiesYarn();
		CPropertiesYarn(TiXmlElement &Element);
		~CPropertiesYarn(void);

		/// Used for saving data to XML
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType);
	};
}
