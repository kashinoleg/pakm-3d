#pragma once
#include "TexGen.h"
using namespace TexGen;
using namespace std;

string ConvertSection(const CSection &Section, string VariableName = "section", bool bRectangle = false );
string ConvertSectionForHybrid( const CSection &Section );
string ConvertWeave3DPattern(const CTextileWeave3D &Weave, string PythonVariableName);
string ConvertWeaveLayerPattern( const CTextileWeave2D &Weave, string PythonVariableName, int iNumLayers );
string ConvertWeaveYarnDimensions(const CTextileWeave &Weave, string PythonVariableName);
string ConvertWeaveYarnDimensions(const CTextile3DWeave &Weave, string PythonVariableName);
string ConvertLayeredWeave( string PythonVariableName, string LayeredName, int iNumLayers, double dOffset);
string ConvertMultiWeaveLayered( vector<string> &LayerNames );
