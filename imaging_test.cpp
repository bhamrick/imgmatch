#include<cstdio>
#include<cstdlib>
#include<imaging.h>

using namespace std;

int main() {
	image_t* img = init(15,15);
	color_t white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255; 
	int x[3] = {0,3,10}, y[3] = {0,13,4};
	fill_polygon(img,3,x,y,white);
	for(int j = 0; j<img->h; j++) {
		for(int i = 0; i<img->w; i++) {
			printf("%d\t",img->data[3*(j*img->w+i)]);
		}
		printf("\n");
	}
	return 0;
}
