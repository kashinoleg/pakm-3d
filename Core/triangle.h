#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct triangulateio {
	double *pointlist;
	double *pointattributelist;
	int *pointmarkerlist;
	int numberofpoints;
	int numberofpointattributes;

	int *trianglelist;
	double *triangleattributelist;
	double *trianglearealist;
	int *neighborlist;
	int numberoftriangles;
	int numberofcorners;
	int numberoftriangleattributes;

	int *segmentlist;
	int *segmentmarkerlist;
	int numberofsegments;

	double *holelist;
	int numberofholes;

	double *regionlist;
	int numberofregions;

	int *edgelist;
	int *edgemarkerlist;
	double *normlist;
	int numberofedges;
};

void triangulate(char *, struct triangulateio *, struct triangulateio *, struct triangulateio *);
void trifree(void *memptr);