#pragma once
#include "Misc.h"

namespace TexGen {
	using namespace std;

	#define NUM_REED_SIZES		30
	static double ReedThickness[NUM_REED_SIZES] = { 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2,
										0.22, 0.24, 0.26, 0.28, 0.31, 0.32, 0.34,
										0.36, 0.37, 0.4, 0.45, 0.5, 0.6,
										0.65, 0.7, 0.8,
										1.0, 1.25, 1.6 };

	class CLASS_DECLSPEC CReedData {
	public:
		CReedData();
		virtual ~CReedData(void);
		double GetClosestReedThickness(double dThickness);
	};
}
