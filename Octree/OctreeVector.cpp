#include "OctreeVector.hpp"

using namespace hxa;

/// plus minus -----------------------------------------------------------------
OctreeVector OctreeVector::operator+(const OctreeVector& v) const {
	return OctreeVector(x_m + v.x_m, y_m + v.y_m, z_m + v.z_m);
}

OctreeVector& OctreeVector::operator+=(const OctreeVector& v) {
	x_m += v.x_m;
	y_m += v.y_m;
	z_m += v.z_m;
	return *this;
}

OctreeVector OctreeVector::operator-(const OctreeVector& v) const {
	return OctreeVector(x_m - v.x_m, y_m - v.y_m, z_m - v.z_m);
}

OctreeVector& OctreeVector::operator-=(const OctreeVector& v) {
	x_m -= v.x_m;
	y_m -= v.y_m;
	z_m -= v.z_m;
	return *this;
}

/// mult div -------------------------------------------------------------------

OctreeVector OctreeVector::operator*(const float f) const {
	return OctreeVector(x_m * f, y_m * f, z_m * f);
}

OctreeVector& OctreeVector::operator*= (const float f) {
	x_m *= f;
	y_m *= f;
	z_m *= f;
	return *this;
}

/// logical --------------------------------------------------------------------
bool OctreeVector::operator==(const OctreeVector& v) const {
	return (x_m == v.x_m) & (y_m == v.y_m) & (z_m == v.z_m);
}

bool OctreeVector::operator!=(const OctreeVector& v) const {
	return (x_m != v.x_m) | (y_m != v.y_m) | (z_m != v.z_m);
}

OctreeVector OctreeVector::operator> (const OctreeVector& v) const {
	return OctreeVector(float(x_m > v.x_m), float(y_m > v.y_m), float(z_m > v.z_m));
}

OctreeVector OctreeVector::operator>=(const OctreeVector& v) const {
	return OctreeVector(float(x_m >= v.x_m), float(y_m >= v.y_m), float(z_m >= v.z_m));
}

OctreeVector OctreeVector::operator<(const OctreeVector& v) const {
	return OctreeVector(float(x_m < v.x_m), float(y_m < v.y_m), float(z_m < v.z_m));
}

OctreeVector OctreeVector::operator<=(const OctreeVector& v) const{
	return OctreeVector(float(x_m <= v.x_m), float(y_m <= v.y_m), float(z_m <= v.z_m));
}

const OctreeVector& OctreeVector::HALF() {
	static const OctreeVector k(0.5f, 0.5f, 0.5f);
	return k;
}
