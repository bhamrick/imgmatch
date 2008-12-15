#ifndef IMAGING_H
#define IMAGING_H

#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<string.h>

struct image_t {
	int w, h;
	unsigned char* data;
};

struct bitmap_t {
	int w, h;
	bool* data;
};

struct color_t {
	unsigned char r, g, b, a;
};

image_t* init(int, int);
bitmap_t* init_bm(int, int);
void destroy(image_t*);
void destroy(bitmap_t*);
bool same_side(int, int, int, int, int, int, int, int);
void xor_triangle(image_t*, int*, int*);
void fill_polygon(image_t*, int, int*, int*, color_t);
int min(int,int);
int max(int,int);

#endif
