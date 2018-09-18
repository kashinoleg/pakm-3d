%module Renderer

%include <stl.i>
%{
	#include "../TexGenRenderer.h"
	using namespace TexGen;
%}

namespace TexGen {}

using namespace std;
using namespace TexGen;

#define CLASS_DECLSPEC

%import "../../Core/Properties.h"
%import "../../Core/PropertiesTextile.h"
%import "../../Core/PropertiesYarn.h"
%import "../../Core/mymath.h"
%import "../../Core/Yarn.h"
%import "../../Core/Textile.h"
%import "../../Core/Domain.h"
%warnfilter(401);
%include "../../Core/Misc.h"
%warnfilter(+401);

%include "../TexGenRenderer.h"
