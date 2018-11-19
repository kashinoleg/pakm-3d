%module Embedded

%include <stl.i>
%{
	#include "EmbeddedPythonFunctions.h"
%}

namespace TexGen {}

using namespace std;
using namespace TexGen;

%include "EmbeddedPythonFunctions.h"