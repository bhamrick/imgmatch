#include<ctime>
#include<cstdio>
#include<cairo.h>
#include<cstdlib>
#include<algorithm>

#define NVERT 5
#define NPOLY 50
#define PMUT 0.03
#define NPOP 16
#define MAXMOVE 15
#define MAXCMOVE 0.2

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

bool operator<(polyimg a, polyimg b) {
	return false;
}

cairo_surface_t* draw_img(polyimg p) {
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
	return ans;
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
			if((double)rand()/RAND_MAX < 0.5) init_polygon(p.poly[i],p.w,p.h);
			else {
				for(int j = 0; j<NVERT; j++) {
					p.poly[i].x[j]+=((double)rand()/RAND_MAX-0.5) * 2 * MAXMOVE;
					p.poly[i].y[j]+=((double)rand()/RAND_MAX-0.5) * 2 * MAXMOVE;
					if(p.poly[i].x[j] < 0) p.poly[i].x[j]=0;
					if(p.poly[i].y[j] < 0) p.poly[i].y[j]=0;
					if(p.poly[i].x[j] > p.w) p.poly[i].x[j]=p.w;
					if(p.poly[i].y[j] > p.h) p.poly[i].y[j]=p.h;
				}
				p.poly[i].r+=((double)rand()/RAND_MAX-0.5) * 2 * MAXCMOVE;
				p.poly[i].g+=((double)rand()/RAND_MAX-0.5) * 2 * MAXCMOVE;
				p.poly[i].b+=((double)rand()/RAND_MAX-0.5) * 2 * MAXCMOVE;
				p.poly[i].a+=((double)rand()/RAND_MAX-0.5) * 2 * MAXCMOVE;
				if(p.poly[i].r < 0) p.poly[i].r = 0;
				if(p.poly[i].g < 0) p.poly[i].g = 0;
				if(p.poly[i].b < 0) p.poly[i].b = 0;
				if(p.poly[i].a < 0) p.poly[i].a = 0;
				if(p.poly[i].r > 1) p.poly[i].r = 1;
				if(p.poly[i].g > 1) p.poly[i].g = 1;
				if(p.poly[i].b > 1) p.poly[i].b = 1;
				if(p.poly[i].a > 1) p.poly[i].a = 1;
			}
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
	int w = cairo_image_surface_get_width(target), h = cairo_image_surface_get_height(target), gen=0;
	polyimg pop[NPOP];
	for(int i = 0; i<NPOP; i++) init_polyimg(pop[i],w,h);
	pair<double,polyimg> tpop[NPOP];
	while(1) {
		gen++;
		printf("Generation %d\n",gen);
		for(int i = 0; i<NPOP; i++) {
			tpop[i].second = pop[i];
			cairo_surface_t *surface = draw_img(pop[i]);
			tpop[i].first = diff(cairo_image_surface_get_data(surface),tdata,w*h*4);
			cairo_surface_destroy(surface);
		}
		printf("Best error: %lf\n",tpop[0].first);
		sort(tpop,tpop+NPOP);
		if(gen%100==1) save_polyimg(tpop[0].second,(char*)"out.png");
		pop[0] = tpop[0].second;
		for(int i = 1; i<NPOP; i++) {
			double x = (double)rand()/RAND_MAX, v = 0.5;
			int a = 1, b = 1;
			while(a < NPOP-1 && v > x) {
				v*=0.5;
				a++;
			}
			x = (double)rand()/RAND_MAX, v = 0.5;
			while(b < NPOP-1 && v > x) {
				v*=0.5;
				b++;
			}
			pop[i] = tpop[a].second;
			crossover(pop[i],tpop[b].second);
			mutate(pop[i]);
		}
	}
	return 0;
}
