#include<cstdio>
#include<vector>
#include<cstdlib>
#include"drawing.h"

using namespace std;

int main() {
	int width = 100, height = 100;
	color *buf = new color[width*height], c;
	for(int i = 0; i<width*height; i++) {
		buf[i].r = buf[i].g = buf[i].b = 0;
	}
	c.r = 255;
	c.g = 0;
	c.b = 0;
	c.a = 0;
	vector< pair<int,int> > p;
	p.push_back(make_pair(10,10));
	p.push_back(make_pair(10,90));
	p.push_back(make_pair(73,56));
	draw_polygon(p, c, buf, width, height);
	p.pop_back();
	p.push_back(make_pair(63,0));
	c.r = 0;
	c.g = 255;
	c.a = 0.5;
	draw_polygon(p,c,buf,width,height);
	p.clear();
	p.push_back(make_pair(50,50));
	p.push_back(make_pair(60,30));
	p.push_back(make_pair(93,70));
	p.push_back(make_pair(93,75));
	p.push_back(make_pair(72,83));
	c.g = 0;
	c.b = 255;
	c.a = 0.25;
	draw_polygon(p,c,buf,width,height);
	FILE *fout = fopen("test.ppm","w");
	fprintf(fout,"P3\n%d %d\n255\n",width,height);
	for(int y = 0; y<height; y++) {
		for(int x = 0; x<width; x++) {
			fprintf(fout,"%d %d %d\n",(int)buf[x*height+y].r,(int)buf[x*height+y].g,(int)buf[x*height+y].b);
		}
	}
	fclose(fout);
	return 0;
}
