#include "PropertiesYarn.h"

using namespace TexGen;

CPropertiesYarn::CPropertiesYarn() {
}

CPropertiesYarn::CPropertiesYarn(TiXmlElement &Element)
: CProperties(Element) {
}

CPropertiesYarn::~CPropertiesYarn(void) {
}

void CPropertiesYarn::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) {
	CProperties::PopulateTiXmlElement(Element, OutputType);
}
