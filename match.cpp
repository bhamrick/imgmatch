#include<ctime>
#include<cstdio>
#include<cairo.h>
#include<cstdlib>

#define NVERT 3
#define NPOLY 20
#define PMUT 0.03
#define NPOP 16

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

unsigned char* img_data(polyimg p) {
	cairo_surface_t *ans;
	ans = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, p.w, p.h);
	cairo_t *cr;
	cr = cairo_create(ans);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_paint(cr);
	for(int i = 0; i<NPOLY; i++) {
		cairo_set_source_rgba(cr,p.poly[i].r,p.poly[i].g,p.poly[i].b,p.poly[i].a);
		cairo_new_path(cr);
		cairo_move_to(cr,p.poly[i].x[0],p.poly[i].y[0]);
		for(int j = 1; j<NVERT; j++) {
			cairo_line_to(cr,p.poly[i].x[j],p.poly[i].y[j]);
		}
		cairo_close_path(cr);
		cairo_fill(cr);
	}
	cairo_destroy(cr);
	unsigned char* data = cairo_image_surface_get_data(ans);
	cairo_surface_destroy(ans);
	return data;
}

void save_polyimg(polyimg p, char* foutname) {
	cairo_surface_t *ans;
	ans = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, p.w, p.h);
	cairo_t *cr;
	cr = cairo_create(ans);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_paint(cr);
	for(int i = 0; i<NPOLY; i++) {
		cairo_set_source_rgba(cr,p.poly[i].r,p.poly[i].g,p.poly[i].b,p.poly[i].a);
		cairo_new_path(cr);
		cairo_move_to(cr,p.poly[i].x[0],p.poly[i].y[0]);
		for(int j = 1; j<NVERT; j++) {
			cairo_line_to(cr,p.poly[i].x[j],p.poly[i].y[j]);
		}
		cairo_close_path(cr);
		cairo_fill(cr);
	}
	cairo_destroy(cr);
	cairo_surface_write_to_png(ans,foutname);
	cairo_surface_destroy(ans);
}

void init_polygon(polygon& p, int w, int h) {
	for(int i = 0; i<NVERT; i++) {
		p.x[i] = (double)rand()/RAND_MAX * w;
		p.y[i] = (double)rand()/RAND_MAX * h;
	}
	p.r = (double)rand()/RAND_MAX;
	p.g = (double)rand()/RAND_MAX;
	p.b = (double)rand()/RAND_MAX;
	p.a = (double)rand()/RAND_MAX;
}

void init_polyimg(polyimg& p, int w, int h) {
	for(int i = 0; i<NPOLY; i++) {
		init_polygon(p.poly[i],w,h);
	}
	p.w = w;
	p.h = h;
}

void mutate(polyimg& p) {
	for(int i = 0; i<NPOLY; i++) {
		if((double)rand()/RAND_MAX < PMUT) {
			init_polygon(p.poly[i],p.w,p.h);
		}
	}
}

void crossover(polyimg& p1, polyimg p2) {
	int c = (int)(((double)rand()/RAND_MAX)*NPOLY);
	for(int i = c; i<NPOLY; i++) {
		p1.poly[i] = p2.poly[i];
	}
}

int main(int argc, char** argv) {
	if(argc == 1) {
		printf("Usage: %s filename\n",argv[0]);
		return 0;
	}
	srand(time(NULL));
	cairo_surface_t *target = cairo_image_surface_create_from_png(argv[1]);
	unsigned char* tdata = cairo_image_surface_get_data(target);
	int w = cairo_image_surface_get_width(target), h = cairo_image_surface_get_height(target);
	polyimg pop[NPOP];
	for(int i = 0; i<NPOP; i++) init_polyimg(pop[i],w,h);
	for(int i = 0; i<NPOP; i++) {
		char foutname[20];
		sprintf(foutname,"img%d.png",i);
		save_polyimg(pop[i],foutname);
		printf("%d: %.3lf\n",i,diff(img_data(pop[i]),tdata,w*h*4));
	}
	return 0;
}
