%module Export

%include <stl.i>
%{
	#include "../Exporter.hxx"
	using namespace TexGen;
%}

namespace TexGen {}

using namespace std;
using namespace TexGen;

#define CLASS_DECLSPEC

%import "../../Core/Properties.h"
%import "../../Core/PropertiesTextile.h"
%import "../../Core/PropertiesYarn.h"
%import "../../Core/Yarn.h"
%import "../../Core/Textile.h"
%import "../../Core/Domain.h"

%include "../Exporter.hxx"
