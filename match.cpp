#include<ctime>
#include<cstdio>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include"drawing.h"

#define NVERT 4
#define NPOLY 50
#define NPOP 16
#define PMUT 0.3
#define PADD 0.1
#define PNEW 0.05
#define MAXMOVE 5
#define MAXCMOVE 0.2
#define MAXAMOVE 0.05
#define FRATE 50

using namespace std;

double irandmax = 1.0/RAND_MAX;

double diff(color* data1, color* data2, int size) {
	double ans = 0.0;
	for(int i = 0; i<size; i++) {
		ans+=(data1->r - data2->r)/255*(data1->r - data2->r)/255 + (data1->g - data2->g)/255*(data1->g - data2->g)/255 + (data1->b - data2->b)/255*(data1->b - data2->b)/255;
		data1++;
		data2++;
	}
	return ans;
}

struct polygon {
	vector< pair<int,int> > v;
	color c;
};

struct polyimg {
	int w, h;
	vector<polygon> poly;
};

bool operator<(polyimg a, polyimg b) {
	return false;
}

void init_polygon(polygon& p, int w, int h) {
	for(int i = 0; i<3; i++) {
		p.v.push_back(make_pair((int)(rand()*irandmax*w),(int)(rand()*irandmax*h)));
	}
	p.c.r = (double)rand()*irandmax*255;
	p.c.g = (double)rand()*irandmax*255;
	p.c.b = (double)rand()*irandmax*255;
	p.c.a = (double)rand()*irandmax;
}

void init_polyimg(polyimg& p, int w, int h) {
	polygon pol;
	init_polygon(pol,w,h);
	p.poly.push_back(pol);
	p.w = w;
	p.h = h;
}

void mutate(polyimg& p) {
	if(rand()*irandmax < PNEW && p.poly.size() < NPOLY) {
		polygon pol;
		init_polygon(pol,p.w,p.h);
		p.poly.push_back(pol);
	}
	for(int i = 0; i<p.poly.size(); i++) {
		if(rand()*irandmax < PMUT) {
			if(rand()*irandmax < PADD && p.poly[i].v.size() < NVERT) {
				p.poly[i].v.push_back(make_pair((int)(rand()*irandmax*p.w),(int)(rand()*irandmax*p.h)));
			} else {
				int j = (int)(rand()*irandmax*p.poly[i].v.size());
				int dx = (int)((2.0*rand()*irandmax-1.0)*MAXMOVE), dy = (int)((2.0*rand()*irandmax-1.0)*MAXMOVE);
				double dr = (int)((2.0*rand()*irandmax-1.0)*MAXCMOVE), dg = (int)((2.0*rand()*irandmax-1.0)*MAXCMOVE), db = (int)((2.0*rand()*irandmax-1.0)*MAXCMOVE), da = (int)((2.0*rand()*irandmax-1.0)*MAXAMOVE);
				if(rand()*irandmax < 0.5) {
					p.poly[i].v[j].first += dx;
					if(p.poly[i].v[j].first < 0) p.poly[i].v[j].first = 0;
					if(p.poly[i].v[j].first >= p.w) p.poly[i].v[j].first = p.w-1;
					p.poly[i].v[j].second += dy;
					if(p.poly[i].v[j].second < 0) p.poly[i].v[j].second = 0;
					if(p.poly[i].v[j].second >= p.h) p.poly[i].v[j].second = p.h-1;
				} else {
					p.poly[i].c.r += dr;
					if(p.poly[i].c.r < 0) p.poly[i].c.r = 0;
					if(p.poly[i].c.r > 255) p.poly[i].c.r = 255;
					p.poly[i].c.g += dg;
					if(p.poly[i].c.g < 0) p.poly[i].c.g = 0;
					if(p.poly[i].c.g > 255) p.poly[i].c.g = 255;
					p.poly[i].c.b += db;
					if(p.poly[i].c.b < 0) p.poly[i].c.b = 0;
					if(p.poly[i].c.b > 255) p.poly[i].c.b = 255;
					p.poly[i].c.a += da;
					if(p.poly[i].c.a < 0) p.poly[i].c.a = 0;
					if(p.poly[i].c.a > 1) p.poly[i].c.a = 1;
				}
			}
		}
	}
}

void crossover(polyimg& p1, polyimg p2) {
	int c1 = (int)(((double)rand()*irandmax)*p1.poly.size());
	vector<polygon> p;
	for(int i = 0; i<c1; i++) {
		p.push_back(p1.poly[i]);
	}
	for(int i = c1; i<p2.poly.size(); i++) {
		p.push_back(p2.poly[i]);
	}
	p1.poly = p;
}

void draw_polyimg(color* buf, polyimg p) {
	for(int i = 0; i<p.poly.size(); i++) {
		draw_polygon(p.poly[i].v,p.poly[i].c,buf,p.w,p.h);
	}
}

void save_polyimg(polyimg& p, char* fname) {
	FILE *fout = fopen(fname,"w");
	fprintf(fout,"P3\n%d %d\n255\n",p.w,p.h);
	color *buf = new color[p.w*p.h];
	draw_polyimg(buf,p);
	for(int y = 0; y<p.h; y++) {
		for(int x = 0; x<p.w; x++) {
			int i = x*p.h + y;
			fprintf(fout,"%d %d %d\n",(int)buf[i].r,(int)buf[i].g,(int)buf[i].b);
		}
	}
	fclose(fout);
	delete[] buf;
}

int main(int argc, char** argv) {
	if(argc == 1) {
		printf("Usage: %s filename\n",argv[0]);
		return 0;
	}
	srand(time(NULL));
	color* target;
	int h, w, gen = 0;
	FILE *fin = fopen(argv[1],"r");
	char s[5];
	fscanf(fin,"%s",s);
	fscanf(fin,"%d%d",&w,&h);
	int foo;
	fscanf(fin,"%d",&foo);
	target = new color[h*w];
	for(int y = 0; y<h; y++) {
		for(int x = 0; x<w; x++) {
			int i = x*h+y;
			int r, g, b;
			fscanf(fin,"%d%d%d",&r,&g,&b);
			target[i].r = r;
			target[i].g = g;
			target[i].b = b;
		}
	}
	printf("%d %d\n",w,h);
	polyimg pop[NPOP];
	for(int i = 0; i<NPOP; i++) init_polyimg(pop[i],w,h);
	pair<double,polyimg> tpop[NPOP];
	while(1) {
		gen++;
		for(int i = 0; i<NPOP; i++) {
			color *buf = new color[w*h];
			tpop[i].second = pop[i];
			draw_polyimg(buf,pop[i]);
			tpop[i].first = diff(buf,target,w*h);
			delete[] buf;
		}
		sort(tpop,tpop+NPOP);
		if(gen%FRATE==1) {
			char foutname[100];
			save_polyimg(tpop[0].second,(char*)"out.ppm");
			sprintf(foutname,"movie/out%07d.ppm",gen/FRATE);
			save_polyimg(tpop[0].second,foutname);
			printf("Generation %d\tBest error: %.0lf\n",gen,tpop[0].first);
			fflush(stdout);
		}
		pop[0] = tpop[0].second;
		for(int i = 1; i<NPOP/3; i++) {
			pop[1] = tpop[i].second;
			mutate(pop[1]);
		}
		for(int i = NPOP/3; i<NPOP-1; i++) {
			if((double)rand()*irandmax < 0.5) {
				pop[i] = tpop[i].second;
				crossover(pop[i],tpop[0].second);
				mutate(pop[i]);
			} else {
				pop[i] = tpop[0].second;
				crossover(pop[i],tpop[i].second);
				mutate(pop[i]);
			}
		}
		pop[NPOP-1] = tpop[0].second;
		mutate(pop[NPOP-1]);
	}
	return 0;
}
