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
namespace TexGen
{
	class CExporter
	{
	private:
		static opencascade::handle<TDocStd_Document> ShapesToDocument(const vector<TopoDS_Solid> shapes);

		static TopoDS_Solid ShapeToSolid(TopoDS_Shape pShape);

		/**
		 * Преобразование нити в твердое тело с учетом повторов в домене.
		 * @param yarn - исходная нить.
		 * @param domain - исходный домен матрицы.
		 * @return - твердотельное представление нити.
		 */
		static TopoDS_Solid meshToSolid(const CMesh mesh);

		/**
		 * Преобразование текстиля в набор твердых тел.
		 * @param textile - исходный текстиль.
		 * @return - набор твердых тел текстиля.
		 */
		static vector<TopoDS_Solid> TextileToVector(CTextile &Textile, bool ExportDomain, bool SubtractYarns);

		/**
		 * Подрезка нитей до размера домена.
		 * @param yarns - исходные нити.
		 * @param domain - исходный домен.
		 * @return - подрезанные нити.
		 */
		static vector<TopoDS_Solid> confineYarnsToDomain(const vector<TopoDS_Solid> yarns, const TopoDS_Solid domain);

		/**
		 * Вырезка нитей из домена матрицы.
		 * @param domain - домен матрицы.
		 * @param yarns - исходные нити.
		 * @return - домен из которого вырезаны нити.
		 */
		static TopoDS_Solid subtractYarnsFromDomain(const TopoDS_Solid domain, const vector<TopoDS_Solid> yarns);

		/**
		* Сохранение твердых тел текстиля файла в формате STEP.
		* @param fileName - имя файла.
		* @param shapes - твердые тела текстиля.
		* @return - сохранен ли файл.
		*/
		static bool saveToSTEP(const string fileName, const vector<TopoDS_Solid> shapes);

		/**
		* Сохранение твердых тел текстиля файла в формате IGES.
		* @param fileName - имя файла.
		* @param shapes - твердые тела текстиля.
		* @return - сохранен ли файл.
		*/
		static bool saveToIGES(const string fileName, const vector<TopoDS_Solid> shapes);
	public:
		/// Output the textile to IGES file format
		static void TextileToIGES(string fileName, CTextile Textile, bool ExportDomain, bool SubtractYarns);
		/// Output the textile to STEP file format
		static void TextileToSTEP(string fileName, CTextile Textile, bool ExportDomain, bool SubtractYarns);

		/// Output the textile to IGES file format
		static void NameTextileToIGES(string fileName, string TextileName, bool ExportDomain, bool SubtractYarns);
		/// Output the textile to STEP file format
		static void NameTextileToSTEP(string fileName, string TextileName, bool ExportDomain, bool SubtractYarns);
	};
}