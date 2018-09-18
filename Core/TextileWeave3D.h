#pragma once
#include "TextileWeave.h"

namespace TexGen { 
	using namespace std;

	/// Respresents a 3d woven textile
	class CLASS_DECLSPEC CTextileWeave3D : public CTextileWeave {
	public:
		CTextileWeave3D(int iWidth, int iHeight, double dSpacing, double dThickness);
		CTextileWeave3D(TiXmlElement &Element);
		~CTextileWeave3D(void);

		CTextile* Copy() const { return new CTextileWeave3D(*this); }
		string GetType() const { return "CTextileWeave3D"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType);

		/// Add given number of yarns parallel to the Y axis, with given index x
		void AddYLayers(int x, int iNumberLayers);
		/// Add given number of yarns parallel to the X axis, with given index y
		void AddXLayers(int y, int iNumberLayers);
		/// Add given number of yarns parallel to the Y axis
		void AddYLayers(int iNumberLayers = 1);
		/// Add given number of yarns parallel to the X axis
		void AddXLayers(int iNumberLayers = 1);
		/// Delete given number of yarns parallel to the Y axis, with given index x
		void DeleteYLayers(int x, int iNumberLayers);
		/// Delete given number of yarns parallel to the X axis, with given index y
		void DeleteXLayers(int y, int iNumberLayers);
		/// Delete given number of yarns parallel to the Y axis
		void DeleteYLayers(int iNumberLayers = 1);
		/// Delete given number of yarns parallel to the X axis
		void DeleteXLayers(int iNumberLayers = 1);
		/// Push down all yarns in the pattern by given number of levels with given index x, y
		void PushDown(int x, int y, int iLevels = 1);
		/// Push up all yarns in the pattern by given number of levels with given index x, y
		void PushUp(int x, int y, int iLevels = 1);
		/// Push down all yarns parallel to the Y axis in the pattern by given number of levels with given index x
		void PushYDown(int x, int iLevels = 1);
		/// Push up all yarns parallel to the Y axis in the pattern by given number of levels with given index x
		void PushYUp(int x, int iLevels = 1);
		/// Push down all yarns parallel to the X axis in the pattern by given number of levels with given index y
		void PushXDown(int y, int iLevels = 1);
		/// Push up all yarns parallel to the X axis in the pattern by given number of levels with given index y
		void PushXUp(int y, int iLevels = 1);
		/// Swap the positions of two yarns in the patter with given index x, y and level iLevel1, iLevel2
		void SwapPosition(int x, int y, int iLevel1, int iLevel2);

		/// Retreive the number of yarns parallel to the Y axis, with given index x
		int GetNumYLayers(int x) const;
		/// Retreive the number of yarns parallel to the X axis, with given index y
		int GetNumXLayers(int y) const;
		/// Retreive the maximum number of layers at any crossover
		int GetMaxNumLayers() const;

		string GetDefaultName() const;
	};
}
