#include<cstdio>
#include<cairo.h>
#include<cstdlib>

#define NVERT 3
#define NPOLY 20

using namespace std;

double diff(unsigned char* data1, unsigned char* data2, int size) {
	double ans = 0.0;
	for(int i = 0; i<size; i++) {
		ans += 0.5*((double)*data1-(double)*data2)*((double)*data1-(double)*data2);
		data1++;
		data2++;
	}
	return ans;
}

struct polygon {
	double x[NVERT], y[NVERT], r, g, b, a;
};

struct polyimg {
	int w, h;
	polygon poly[NPOLY];
};

unsigned char* img_data(polyimg *p) {
	cairo_surface_t *ans;
	ans = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, p->w, p->h);
	cairo_t *cr;
	cr = cairo_create(ans);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_paint(cr);
	for(int i = 0; i<NPOLY; i++) {
		cairo_set_source_rgba(cr,p->poly[i].r,p->poly[i].g,p->poly[i].b,p->poly[i].a);
		cairo_new_path(cr);
		cairo_move_to(cr,p->poly[i].x[0],p->poly[i].y[0]);
		for(int j = 1; j<NVERT; j++) {
			cairo_line_to(cr,p->poly[i].x[j],p->poly[i].y[j]);
		}
		cairo_close_path(cr);
		cairo_fill(cr);
	}
	cairo_destroy(cr);
	unsigned char* data = cairo_image_surface_get_data(ans);
	cairo_surface_destroy(ans);
	return data;
}

int main(int argc, char** argv) {
	if(argc == 1) {
		printf("Usage: %s filename\n",argv[0]);
		return 0;
	}
	cairo_surface_t *target = cairo_image_surface_create_from_png(argv[1]);
	unsigned char* tdata = cairo_image_surface_get_data(target);
	
}
