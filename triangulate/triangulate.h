#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "triangulateio.h"

namespace trg
{
	class CTriangulate
	{
	public:
		static void triangulate(std::string, CTriangulateIO *, CTriangulateIO *, CTriangulateIO *);
		static void trifree(void *memptr);
	};
}
