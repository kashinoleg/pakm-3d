#pragma once
#include <algorithm>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <cstring>

namespace latin {
	using namespace std;
	int get_seed();
	void latin_random(int dim_num, int point_num, int *seed, double x[]);
	int i4_uniform(int ilo, int ihi, int *seed);
	int *perm_uniform(int n, int base, int *seed);
	int r4_nint(float x);
	double r8_uniform_01(int *seed);
}