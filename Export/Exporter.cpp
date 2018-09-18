#include "Exporter.hxx"

using namespace TexGen;

CExporter::CExporter() {
	m_bExportDomain = true;
	m_bSubtractYarns = false;
}

bool CExporter::OutputDomainToIGES(string fileName, CDomain &Domain) {
	TGLOGINDENT("Saving Domain to IGES file: " << fileName);
	vector<TopoDS_Solid> shapes;
	CMesh mesh = Domain.GetMesh();
	TopoDS_Solid domain = meshToSolid(mesh);
	shapes.push_back(domain);
	return saveToIGES(fileName, shapes);
}

bool CExporter::OutputDomainToSTEP(string fileName, CDomain &Domain) {
	TGLOGINDENT("Saving Domain to STEP file: " << fileName);
	vector<TopoDS_Solid> shapes;
	CMesh mesh = Domain.GetMesh();
	TopoDS_Solid domain = meshToSolid(mesh);
	shapes.push_back(domain);
	return saveToSTEP(fileName, shapes);
}

bool CExporter::OutputTextileToIGES(string fileName, CTextile &Textile) {
	TGLOGINDENT("Saving Textile to IGES file: " << fileName);
	vector<TopoDS_Solid> shapes = TextileToVector(Textile);
	bool status = saveToIGES(fileName, shapes);
	if (status) {
		TGLOGINDENT("IGES textile file " << fileName << " saved");
	}
	else {
		TGLOGINDENT("Error: IGES textile file " << fileName << " not saved");
	}
	return status;
}

bool CExporter::OutputTextileToSTEP(string fileName, CTextile &Textile) {
	TGLOGINDENT("Saving Textile to STEP file: " << fileName.c_str());
	vector<TopoDS_Solid> shapes = TextileToVector(Textile);
	bool status = saveToSTEP(fileName, shapes);
	if (status) {
		TGLOGINDENT("STEP textile file " << fileName.c_str() << " saved");
	}
	else {
		TGLOGINDENT("Error: STEP textile file " << fileName.c_str() << " not saved");
	}
	return status;
}

bool CExporter::OutputTextileToIGES(string fileName, string TextileName) {
	CTextile *pTextile = CTexGen::GetInstance().GetTextile(TextileName);
	if (!pTextile) {
		return false;
	}
	else {
		return OutputTextileToIGES(fileName, *pTextile);
	}
}

bool CExporter::OutputTextileToSTEP(string fileName, string TextileName) {
	CTextile *pTextile = CTexGen::GetInstance().GetTextile(TextileName);
	if (!pTextile) {
		return false;
	}
	else {
		return OutputTextileToSTEP(fileName, *pTextile);
	}
}

opencascade::handle<TDocStd_Document> CExporter::ShapesToDocument(const vector<TopoDS_Solid> solids) {
	TGLOGINDENT("Write shapes of textile to file");
	Handle(TDocStd_Document) aDoc;
	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	anApp->NewDocument("MDTV-CAF", aDoc);
	int i = 0;
	for (TopoDS_Solid solid : solids) {
		if (!solid.IsNull()) {
			BRepCheck_Analyzer valid(solid);
			if (valid.IsValid()) {
				TGLOG("Create " << ++i << " shapes to document from " << solids.size());
				// Create label and add our shape
				Handle(XCAFDoc_ShapeTool) myShapeTool = XCAFDoc_DocumentTool::ShapeTool(aDoc->Main());
				TDF_Label aLabel = myShapeTool->NewShape();
				myShapeTool->SetShape(aLabel, solid);
			}
			else {
				TGLOG("Unvalid " << ++i << " shapes to document from " << solids.size());
			}
		}
		else {
			TGLOG("Null " << ++i << " shapes to document from " << solids.size());
		}	
	}
	return aDoc;
}

bool CExporter::saveToSTEP(const string fileName, const vector<TopoDS_Solid> shapes) {
	Handle(TDocStd_Document) aDoc = ShapesToDocument(shapes);
	STEPCAFControl_Writer myWriter;
	Standard_Boolean result;
	try {
		ConvertUTF coder;
		wstring wstr = coder.utf8_decode(fileName);
		result = myWriter.Perform(aDoc, wstr.c_str());
	}
	catch (...) {
		TGERROR("Error write shapes to file");
		return false;
	}
	return result;
}

bool CExporter::saveToIGES(const string fileName, const vector<TopoDS_Solid> shapes) {
	Handle(TDocStd_Document) aDoc = ShapesToDocument(shapes);
	IGESCAFControl_Writer myWriter;
	Standard_Boolean result;
	try {
		ConvertUTF coder;
		wstring wstr = coder.utf8_decode(fileName);
		result = myWriter.Perform(aDoc, wstr.c_str());
	}
	catch (...) {
		TGERROR("Error write shapes to file");
		return false;
	}
	return result;
}

vector<TopoDS_Solid> CExporter::TextileToVector(CTextile &Textile) {
	TGLOGINDENT("Convert to Open CASCADE format");
	vector<TopoDS_Solid> shapes;
	bool isDomain = m_bExportDomain && Textile.GetDomain() != NULL;
	int i = 0;
	vector<CYarn> yarns = Textile.GetYarns();
	for (CYarn yarn : yarns) {
		i++;
		TGLOG("Converting yarn: " << i << "/" << yarns.size());
		CMesh mesh = CMesh();
		if (isDomain) {
			yarn.AddSurfaceToMesh(mesh, Textile.GetDomain()->GetTranslations(yarn), true);
		}
		else {
			yarn.AddSurfaceToMesh(mesh, true);
		}
		TopoDS_Solid solid = meshToSolid(mesh);
		if (!solid.IsNull()) {
			BRepCheck_Analyzer valid(solid);
			if (valid.IsValid()) {
				shapes.push_back(solid);
			}
			else {
				TGLOG("Unvalid " << i << " shapes from " << yarns.size());
			}
		}
		else {
			TGLOG("Null " << i << " shapes from " << yarns.size());
		}
	}
	if (isDomain) {
		TGLOG("Converting domain");
		CMesh mesh = Textile.GetDomain()->GetMesh();
		TopoDS_Solid domain = meshToSolid(mesh);
		shapes = confineYarnsToDomain(shapes, domain);
		if (m_bSubtractYarns) {
			domain = subtractYarnsFromDomain(domain, shapes);
		}
		shapes.push_back(domain);
	}
	return shapes;
}

vector<TopoDS_Solid> CExporter::confineYarnsToDomain(const vector<TopoDS_Solid> yarns, const TopoDS_Solid domain) {
	TGLOGINDENT("Confining yarns to the domain");
	vector<TopoDS_Solid> shapes;
	int i = 0;
	for (TopoDS_Solid yarn : yarns) {
		TGLOG("Confining yarn: " << ++i << "/" << yarns.size());
		if (!yarn.IsNull() && !domain.IsNull()) {
			try {
				BRepAlgoAPI_Common Common(yarn, domain);
				if (!Common.HasErrors()) {
					shapes.push_back(ShapeToSolid(Common.Shape()));
				}
				else {
					TGERROR("Failed in confining " << i << " yarn");
				}
			}
			catch (...) {
				TGERROR("Error in confining " << i << " yarn");
			}
		}
		else {
			TGLOG("Warning: yarn has not confined, yarn or domain is null");
		}
	}
	return shapes;
}

TopoDS_Solid CExporter::subtractYarnsFromDomain(const TopoDS_Solid domain, const vector<TopoDS_Solid> yarns) {
	TGLOGINDENT("Cutting yarns out of the domain");
	TopoDS_Solid shape = domain;
	int i = 0;
	for (TopoDS_Solid yarn : yarns) {
		TGLOG("Cutting domain with yarn: " << ++i << "/" << yarns.size());
		if (!yarn.IsNull() && !shape.IsNull()) {
			try {
				BRepAlgoAPI_Cut Cut(shape, yarn);
				if (!Cut.HasErrors()) {
					shape = ShapeToSolid(Cut.Shape());
				}
				else {
					TGERROR("Failed to confine domain to yarn " << i);
				}
			}
			catch (...) {
				TGERROR("Error to confine domain to yarn " << i);
			}
		}
		else {
			TGLOG("Warning: domain has not cutted, yarn or domain is null");
		}
	}
	return shape;
}

TopoDS_Solid CExporter::ShapeToSolid(TopoDS_Shape pShape) {
	/*TopoDS_Solid solid;
	try {
		solid = TopoDS::Solid(pShape);
	}
	catch (...) {
		TGERROR("Error convert shape to solid");
	}
	return solid;//*/
	// error check
	if (pShape.IsNull())
		return TopoDS_Solid();

	BRep_Builder lBuilder;
	BRepBuilderAPI_Sewing lFaceSewer;
	TopoDS_Shell lSewedShell;
	TopoDS_Solid lSolid;
	// make solids
	lBuilder.MakeSolid(lSolid);
	
	// explore all faces from source and add it to the target
	for (TopExp_Explorer lExplorer(pShape, TopAbs_ShapeEnum::TopAbs_FACE); lExplorer.More(); lExplorer.Next())
		lFaceSewer.Add(TopoDS::Face(lExplorer.Current()));
	//TGLOG("Perform");
	// perform
	lFaceSewer.SetTolerance(0.001);
	lFaceSewer.Perform();
	//TGLOG("Check");
	// check
	lSewedShell = TopoDS::Shell(lFaceSewer.SewedShape());
	if (lSewedShell.Closed() == Standard_False)
		return TopoDS_Solid();
	
	lBuilder.Add(lSolid, lSewedShell);
	return lSolid;
	/*TGLOG("Warning: 5");
	TopoDS_Solid lResult;
	lBuilder.MakeSolid(lResult);
	BRepClass3d_SolidClassifier SC(lSolid);
	SC.PerformInfinitePoint(Precision::Confusion());
	TGLOG("Warning: 6");
	if (SC.State() == TopAbs_IN) {
		TGLOG("Warning: 7");
		lBuilder.Add(lResult, lSolid.Reversed());
	}
	else {
		TGLOG("Warning: 8");
		try {
			lBuilder.Add(lResult, lSolid);
		}
		catch (...) {
			TGERROR("Error convert shape to solid");
		}
	}
	return lResult;//*/
}

TopoDS_Solid CExporter::meshToSolid(const CMesh mesh) {
	TopoDS_Solid lSolid;
	BRep_Builder lBuilder;
	lBuilder.MakeSolid(lSolid);
	BRepBuilderAPI_Sewing lFaceSewer;
	for (TriangleFace face : mesh.getFaces()) {
		BRepBuilderAPI_MakePolygon PolygonalWire;
		gp_Pnt pointA(face.pointA.x, face.pointA.y, face.pointA.z);
		PolygonalWire.Add(pointA);
		gp_Pnt pointB(face.pointB.x, face.pointB.y, face.pointB.z);
		PolygonalWire.Add(pointB);
		gp_Pnt pointC(face.pointC.x, face.pointC.y, face.pointC.z);
		PolygonalWire.Add(pointC);
		PolygonalWire.Close();
		TopoDS_Wire wire = PolygonalWire.Wire();
		TopoDS_Face TDFace = BRepBuilderAPI_MakeFace(wire);
		lFaceSewer.Add(TDFace);
	}
	lFaceSewer.SetTolerance(0.00001);
	lFaceSewer.Perform();
	TopoDS_Shell lSewedShell = TopoDS::Shell(lFaceSewer.SewedShape());
	lBuilder.Add(lSolid, lSewedShell);
	return lSolid;
}
