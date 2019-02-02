#include "PythonConverter.h"

string ConvertSection(const CSection &Section, string VariableName, bool bRectangle)
{
	stringstream StringStream;
	bool bRotatedRect = false;

	string Type = Section.GetType();
	if (Type == "CSectionEllipse")
	{
		const CSectionEllipse &Ellipse = (const CSectionEllipse&)Section;
		StringStream << VariableName << " = CSectionEllipse(" << Ellipse.GetWidth() << ", " << Ellipse.GetHeight() << ")" << endl;
	}
	else if (Type == "CSectionLenticular")
	{
		const CSectionLenticular &Lenticular = (const CSectionLenticular&)Section;
		StringStream << VariableName << " = CSectionLenticular(" << Lenticular.GetWidth() << ", " << Lenticular.GetHeight() << ", " << Lenticular.GetDistortion() << ")" << endl;
	}
	else if (Type == "CSectionPowerEllipse")
	{
		const CSectionPowerEllipse &PowerEllipse = (const CSectionPowerEllipse&)Section;
		StringStream << VariableName << " = CSectionPowerEllipse(" << PowerEllipse.GetWidth() << ", " << PowerEllipse.GetHeight() << ", " << PowerEllipse.GetPower() << ", " << PowerEllipse.GetXOffset() << ")" << endl;
	}
	else if (Type == "CSectionRectangle")
	{
		const CSectionRectangle &Rectangle = (const CSectionRectangle&)Section;
		StringStream << VariableName << " = CSectionRectangle(" << Rectangle.GetWidth() << ", " << Rectangle.GetHeight() << ")" << endl;
		StringStream << "SectionMesh = CSectionMeshRectangleSection()" << endl;
		StringStream << VariableName << ".AssignSectionMesh( SectionMesh )" << endl;
	}
	else if (Type == "CSectionHybrid")
	{
		const CSectionHybrid &Hybrid = (const CSectionHybrid&)Section;
		StringStream << VariableName << " = CSectionHybrid()" << endl;
		for ( int i = 0; i < Hybrid.GetNumDivisions() ; i++ )
		{
			StringStream << VariableName << ".AddDivision(" << Hybrid.GetDivision(i) << ")" << endl;
			StringStream << VariableName << ".AssignSection(" << i << "," << ConvertSectionForHybrid(Hybrid.GetSection(i)) << ")" << endl;
		}
	}
	else if (Type == "CSectionRotated")
	{
		const CSectionRotated &Rotated = (const CSectionRotated&)Section;
		StringStream << ConvertSection(Rotated.GetSection(), VariableName);
		StringStream << VariableName << " = CSectionRotated(" << VariableName << ", " << Rotated.GetAngle() << ")" << endl;
		if ( Rotated.GetSection().GetType() == "CSectionRectangle" )
		{
			bRotatedRect = true;
			StringStream << "SectionMesh = CSectionMeshRectangleSection()" << endl;
		}
	}
	else if (Type == "CSectionScaled")
	{
		const CSectionScaled &Scaled = (const CSectionScaled&)Section;
		StringStream << ConvertSection(Scaled.GetSection(), VariableName);
		StringStream << VariableName << " = CSectionScaled(" << VariableName << ", XY(" << Scaled.GetScale() << "))" << endl;
	}

	// If some, but not all, of sections are rectangle sections need to make bTriangleCorners in section mesh false.
	// Otherwise sections will be incompatible when create volume mesh
	if ( bRectangle && Type != "CSectionRectangle" && !bRotatedRect  )
	{
		StringStream << "SectionMesh = CSectionMeshRectangular( -1, False )" << endl;
		StringStream << VariableName << ".AssignSectionMesh( SectionMesh )" << endl;
	}

	return StringStream.str();
}

string ConvertSectionForHybrid(const CSection &Section)
{
	stringstream StringStream;

	string Type = Section.GetType();
	if (Type == "CSectionEllipse")
	{
		const CSectionEllipse &Ellipse = (const CSectionEllipse&)Section;
		StringStream << "CSectionEllipse(" << Ellipse.GetWidth() << ", " << Ellipse.GetHeight() << ")" << endl;
	}
	else if (Type == "CSectionLenticular")
	{
		const CSectionLenticular &Lenticular = (const CSectionLenticular&)Section;
		StringStream << "CSectionLenticular(" << Lenticular.GetWidth() << ", " << Lenticular.GetHeight() << ", " << Lenticular.GetDistortion() << ")" << endl;
	}
	else if (Type == "CSectionPowerEllipse")
	{
		const CSectionPowerEllipse &PowerEllipse = (const CSectionPowerEllipse&)Section;
		StringStream << "CSectionPowerEllipse(" << PowerEllipse.GetWidth() << ", " << PowerEllipse.GetHeight() << ", " << PowerEllipse.GetPower() << ", " << PowerEllipse.GetXOffset() << ")" << endl;
	}
	else if (Type == "CSectionRectangle")
	{
		const CSectionRectangle &Rectangle = (const CSectionRectangle&)Section;
		StringStream << "CSectionRectangle(" << Rectangle.GetWidth() << ", " << Rectangle.GetHeight() << ")" << endl;
	}
	
	return StringStream.str();
}


string ConvertWeave3DPattern(const CTextileWeave3D &Weave, string PythonVariableName)
{
	stringstream StringStream;

	for (int i = 0; i < Weave.GetNumXYarns(); i++) {
		StringStream << PythonVariableName << ".AddXLayers(" << i << ", " << Weave.GetNumXLayers(i) << ")" << endl;
	}
	for (int i = 0; i < Weave.GetNumYYarns(); i++) {
		StringStream << PythonVariableName << ".AddYLayers(" << i << ", " << Weave.GetNumYLayers(i) << ")" << endl;
	}

	int k;
	int iXCount;
	int iYCount;

	vector<bool>::const_iterator itCellStart;
	vector<bool>::const_reverse_iterator itCellEnd;
	int iNumXLayers;
	int iNumYLayers;
	vector<int> Swap1, Swap2;
	for (int i = 0; i < Weave.GetNumYYarns(); i++) {
		iNumYLayers = Weave.GetNumYLayers(i);
		for (int j = 0; j < Weave.GetNumXYarns(); j++) {
			iNumXLayers = Weave.GetNumXLayers(j);
			const vector<bool> &Cell = Weave.GetCell(i, j);
			Swap1.clear();
			Swap2.clear();
			for (itCellStart = Cell.begin(), iXCount=0; itCellStart != Cell.end() && iXCount < iNumXLayers; ++itCellStart, ++iXCount)
			{
				if (*itCellStart == PATTERN_YYARN)
					Swap1.push_back(iXCount);
			}
			for (itCellEnd = Cell.rbegin(), iYCount=0; itCellEnd != Cell.rend() && iYCount < iNumYLayers; ++itCellEnd, ++iYCount)
			{
				if (*itCellEnd == PATTERN_XYARN)
					Swap2.push_back((int)Cell.size()-(iYCount+1));
			}
			assert(Swap1.size() == Swap2.size());
			for (k=0; k<(int)Swap1.size() && k<(int)Swap2.size(); ++k)
			{
				StringStream << PythonVariableName << ".SwapPosition(" << i << ", " << j << ", " << Swap1[k] << ", " << Swap2[k] << ")" << endl;
			}
		}
	}
	return StringStream.str();
}

string ConvertLayeredWeave( string PythonVariableName, string LayeredName, int iNumLayers, double dOffset)
{
	stringstream StringStream;
	StringStream << LayeredName << " = CTextileLayered()" << endl;
	StringStream << "Offset = XYZ()" << endl;
	for (int i = 0; i < iNumLayers; i++) {
		StringStream << LayeredName << ".AddLayer(" << PythonVariableName << ", Offset )" << endl;
		StringStream << "Offset.z += " << dOffset << endl;
	}
	return StringStream.str();
}

string ConvertMultiWeaveLayered(vector<string> &LayerNames)
{
	auto LayeredTextile = new CTextileLayered();
	XYZ Offset;
	XYZ Min;
	XYZ Max;
	for (vector<string>::reverse_iterator itLayerNames = LayerNames.rbegin(); itLayerNames != LayerNames.rend(); itLayerNames++)
	{
		auto textile = new CTextile(*CTexGen::Instance().GetTextile(*itLayerNames));
		CDomain* pDomain = textile->GetDomain();
		pair<XYZ, XYZ> AABB;
		if ( !pDomain )
		{
			string str = "Cannot output textile, " + (*itLayerNames) + ". No domain specified ";
			TGERROR( str );
			continue;
		}
		else
		{
			AABB = pDomain->GetMesh().GetAABB();  
		}

		if ( itLayerNames != LayerNames.rbegin() )
		{
			Offset.z -= AABB.first.z;
			Min.x = Min.x < AABB.first.x ? Min.x : AABB.first.x;
			Min.y = Min.y < AABB.first.y ? Min.y : AABB.first.y;
			Max.x = Max.x > AABB.second.x ? Max.x : AABB.second.x;
			Max.y = Max.y > AABB.second.y ? Max.y : AABB.second.y;
		}
		else
		{
			Min = AABB.first;
			Max = AABB.second;
		}
		auto offset = XYZ(Offset);
		LayeredTextile->AddLayer(*textile, offset);
		Offset.z += AABB.second.z;
	}

	Max.z = Offset.z;
	auto domain = new CDomainPlanes(XYZ(Min), XYZ(Max));
	LayeredTextile->AssignDomain(*domain);
	CTexGen::Instance().AddTextile(*LayeredTextile);
	//StringStream << "Domain = CDomainPlanes( XYZ(" << Min << "), XYZ(" << Max << ") )" << endl;
	//StringStream << "LayeredTextile.AssignDomain( Domain )" << endl;
	//StringStream << "LayeredTextile.SetOffsets( XY(0,0) )" << endl;
	//StringStream << "AddTextile(LayeredTextile)" << endl;
	return "";
}

string ConvertWeaveLayerPattern(const CTextileWeave2D &Weave, string PythonVariableName, int iNumLayers)
{
	stringstream StringStream;

	for (int i = 0; i < iNumLayers; i++) {
		StringStream << PythonVariableName << ".AddXLayers( 1 )" << endl;
		StringStream << PythonVariableName << ".AddYLayers( 1 )" << endl;
	}

	int iWidth = Weave.GetNumYYarns();
	int iHeight = Weave.GetNumXYarns();

	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			if (!Weave.GetCell(i,j)[0]) {
				for (int k = 0; k < iNumLayers; k++) {
					StringStream << "weave.SwapPosition(" << i <<", " << j << ", " << k*2 << ", " << (k*2)+1 << ")" << endl;
				}
			}
		}
	}
	return StringStream.str();
}

string ConvertWeaveYarnDimensions(const CTextileWeave &Weave, string PythonVariableName)
{
	stringstream StringStream;
	for (int i = 0; i < Weave.GetNumXYarns(); i++) {
		StringStream << PythonVariableName << ".SetXYarnWidths(" << i << ", " << Weave.GetXYarnWidths(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetXYarnHeights(" << i << ", " << Weave.GetXYarnHeights(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetXYarnSpacings(" << i << ", " << Weave.GetXYarnSpacings(i) << ")" << endl;
	}
	for (int i = 0; i < Weave.GetNumYYarns(); i++) {
		StringStream << PythonVariableName << ".SetYYarnWidths(" << i << ", " << Weave.GetYYarnWidths(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetYYarnHeights(" << i << ", " << Weave.GetYYarnHeights(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetYYarnSpacings(" << i << ", " << Weave.GetYYarnSpacings(i) << ")" << endl;
	}
	return StringStream.str();
}

string ConvertWeaveYarnDimensions(const CTextile3DWeave &Weave, string PythonVariableName)
{
	stringstream StringStream;
	for (int i = 0; i < Weave.GetNumXYarns(); i++) {
		StringStream << PythonVariableName << ".SetXYarnWidths(" << i << ", " << Weave.GetXYarnWidths(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetXYarnHeights(" << i << ", " << Weave.GetXYarnHeights(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetXYarnSpacings(" << i << ", " << Weave.GetXYarnSpacings(i) << ")" << endl;
	}
	for (int i = 0; i < Weave.GetNumYYarns(); i++) {
		StringStream << PythonVariableName << ".SetYYarnWidths(" << i << ", " << Weave.GetYYarnWidths(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetYYarnHeights(" << i << ", " << Weave.GetYYarnHeights(i) << ")" << endl;
		StringStream << PythonVariableName << ".SetYYarnSpacings(" << i << ", " << Weave.GetYYarnSpacings(i) << ")" << endl;
	}
	return StringStream.str();
}
