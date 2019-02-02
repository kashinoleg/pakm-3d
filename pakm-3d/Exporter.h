#pragma once
#include "TexGen.h"

#include <iostream>
#include <fstream>

#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TColgp_HArray1OfPnt.hxx>

#include <BRepTools.hxx> 
#include <BRep_Builder.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepBuilderAPI_Sewing.hxx>


#include <BRepPrimAPI_MakeBox.hxx>
#include <GeomAPI_Interpolate.hxx>

#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>

#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_Copy.hxx>

#include <Interface_Static.hxx>

#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>

#include <STEPControl_Controller.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_StepModelType.hxx>


#include <TransferBRep_ShapeMapper.hxx>
#include <TransferBRep.hxx>
#include <IGESCAFControl_Writer.hxx>
#include <IGESBasic_Name.hxx>

#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <TopExp.hxx>
#include <STEPCAFControl_Writer.hxx>


#include <XSControl_WorkSession.hxx>

#include <iostream>
#include <gp_Ax2.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <StlAPI_Writer.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepFeat_MakeCylindricalHole.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepCheck_Analyzer.hxx>

#include <BRepClass3d_SolidClassifier.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_Shape.hxx>

#include <math.h>
#include <vector>
#include <list>

#include "Convert.h"
#include <regex>

using namespace std;

/// Class to re-create geometry in OpenCASCADE and in turn export to IGES or STEP
namespace TexGen {
	class CExporter {
	private:
		bool m_bExportDomain;
		bool m_bSubtractYarns;
	private:
		opencascade::handle<TDocStd_Document> ShapesToDocument(const vector<TopoDS_Solid> shapes);
		TopoDS_Solid ShapeToSolid(TopoDS_Shape pShape);
		/**
		 * ѕреобразование нити в твердое тело с учетом повторов в домене.
		 * @param yarn - исходна€ нить.
		 * @param domain - исходный домен матрицы.
		 * @return - твердотельное представление нити.
		 */
		TopoDS_Solid meshToSolid(const CMesh mesh);

		/**
		 * ѕреобразование текстил€ в набор твердых тел.
		 * @param textile - исходный текстиль.
		 * @return - набор твердых тел текстил€.
		 */
		vector<TopoDS_Solid> TextileToVector(CTextile &Textile);

		/**
		 * ѕодрезка нитей до размера домена.
		 * @param yarns - исходные нити.
		 * @param domain - исходный домен.
		 * @return - подрезанные нити.
		 */
		vector<TopoDS_Solid> confineYarnsToDomain(const vector<TopoDS_Solid> yarns, const TopoDS_Solid domain);

		/**
		 * ¬ырезка нитей из домена матрицы.
		 * @param domain - домен матрицы.
		 * @param yarns - исходные нити.
		 * @return - домен из которого вырезаны нити.
		 */
		TopoDS_Solid subtractYarnsFromDomain(const TopoDS_Solid domain, const vector<TopoDS_Solid> yarns);

		/**
		* —охранение твердых тел текстил€ файла в формате STEP.
		* @param fileName - им€ файла.
		* @param shapes - твердые тела текстил€.
		* @return - сохранен ли файл.
		*/
		bool saveToSTEP(const string fileName, const vector<TopoDS_Solid> shapes);

		/**
		* —охранение твердых тел текстил€ файла в формате IGES.
		* @param fileName - им€ файла.
		* @param shapes - твердые тела текстил€.
		* @return - сохранен ли файл.
		*/
		bool saveToIGES(const string fileName, const vector<TopoDS_Solid> shapes);
	public:
		/**
		 *  онструктор по умолчанию.
		 */
		CExporter();

		/**
		 * ƒеструктор по умолчанию.
		 */
		~CExporter() {}

		/// Output the domain to IGES file format
		bool OutputDomainToIGES(string fileName, CDomain &Domain);
		/// Output the domain to STEP file format
		bool OutputDomainToSTEP(string fileName, CDomain &Domain);

		/// Output the textile to IGES file format
		bool OutputTextileToIGES(string fileName, CTextile &Textile);
		/// Output the textile to STEP file format
		bool OutputTextileToSTEP(string fileName, CTextile &Textile);

		/// Output the textile to IGES file format
		bool OutputTextileToIGES(string fileName, string TextileName);
		/// Output the textile to STEP file format
		bool OutputTextileToSTEP(string fileName, string TextileName);

		bool GetExportDomain() { return m_bExportDomain; }
		bool GetSubtractYarns() { return m_bSubtractYarns; }

		void SetExportDomain(bool bExportDomain) { m_bExportDomain = bExportDomain; }
		void SetSubtractYarns(bool bSubtractYarns) { m_bSubtractYarns = bSubtractYarns; }
	};
}