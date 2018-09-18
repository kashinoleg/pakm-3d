#include "latin_random.hpp"
using namespace std;

int latin::get_seed() {
	const int iMax = 2147483647;
	time_t tloc;
	//
	//  If the internal seed is 0, generate a value based on the time.
	//
	time_t clock = time(&tloc);
	struct tm *lt = localtime(&clock);
	//
	//  Hours is 1, 2, ..., 12.
	//
	int ihour = lt->tm_hour;

	if (12 < ihour) {
		ihour = ihour - 12;
	}
	//
	//  Move Hours to 0, 1, ..., 11
	//
	ihour = ihour - 1;

	int imin = lt->tm_min;

	int isec = lt->tm_sec;

	int seed = isec + 60 * (imin + 60 * ihour);
	//
	//  We want values in [1,43200], not [0,43199].
	//
	seed = seed + 1;
	//
	//  Remap SEED from [1,43200] to [1,IMAX].
	//
	seed = (int)
		(((double)seed)
			* ((double)iMax) / (60.0 * 60.0 * 12.0));
	//
	//  Never use a seed of 0.
	//
	if (seed == 0) {
		seed = 1;
	}
	return seed;
}

int latin::i4_uniform(int a, int b, int *seed) {
	if (*seed == 0) {
		cerr << "\n";
		cerr << "I4_UNIFORM - Fatal error!\n";
		cerr << "  Input value of SEED = 0.\n";
		exit(1);
	}

	int k = *seed / 127773;

	*seed = 16807 * (*seed - k * 127773) - k * 2836;

	if (*seed < 0) {
		*seed = *seed + 2147483647;
	}

	float r = (float)(*seed) * 4.656612875E-10;
	//
	//  Scale R to lie between A-0.5 and B+0.5.
	//
	r = (1.0 - r) * ((float)(min(a, b)) - 0.5)
		+ r   * ((float)(max(a, b)) + 0.5);
	//
	//  Use rounding to convert R to an integer between A and B.
	//
	int value = r4_nint(r);

	value = max(value, min(a, b));
	value = min(value, max(a, b));

	return value;
}

void latin::latin_random(int dim_num, int point_num, int *seed, double x[]) {
	int base = 0;
	//
	//  For spatial dimension I, 
	//    pick a random permutation of 1 to POINT_NUM,
	//    force the corresponding I-th components of X to lie in the
	//    interval ( PERM[J]-1, PERM[J] ) / POINT_NUM.
	//
	int k = 0;
	for (int i = 0; i < dim_num; i++) {
		int *perm = perm_uniform(point_num, base, seed);
		for (int j = 0; j < point_num; j++) {
			double r = r8_uniform_01(seed);
			x[k] = (((double)perm[j]) + r) / ((double)point_num);
			k = k + 1;
		}
		delete[] perm;
	}
	return;
}

int* latin::perm_uniform(int n, int base, int *seed) {
	int *p = new int[n];
	for (int i = 0; i < n; i++) {
		p[i] = i + base;
	}
	for (int i = 0; i < n; i++) {
		int j = i4_uniform(i, n - 1, seed);
		int k = p[i];
		p[i] = p[j];
		p[j] = k;
	}
	return p;
}

int latin::r4_nint(float x) {
	if (x < 0.0) {
		return -(int)(abs(x) + 0.5);
	}
	else {
		return (int)(abs(x) + 0.5);
	}
}

double latin::r8_uniform_01(int *seed) {
	if (*seed == 0) {
		cerr << "\n";
		cerr << "R8_UNIFORM_01 - Fatal error!\n";
		cerr << "  Input value of SEED = 0.\n";
		exit(1);
	}

	int k = *seed / 127773;

	*seed = 16807 * (*seed - k * 127773) - k * 2836;

	if (*seed < 0) {
		*seed = *seed + 2147483647;
	}
	//
	//  Although SEED can be represented exactly as a 32 bit integer,
	//  it generally cannot be represented exactly as a 32 bit real number!
	//
	double r = (double)(*seed) * 4.656612875E-10;
	return r;
}
