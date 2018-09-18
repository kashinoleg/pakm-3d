#pragma once

/*	Internal structure for tracking location of items
in the XML file.
*/
class TiXmlCursor
{
public:
	int row;	// 0 based.
	int col;	// 0 based.

	TiXmlCursor() { Clear(); }
	void Clear() { row = col = -1; }
};