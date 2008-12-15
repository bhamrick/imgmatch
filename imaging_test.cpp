#include<cstdio>
#include<cstdlib>
#include<imaging.h>

using namespace std;

int main() {
	image_t* img = init(5,5);
	color_t white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;
	int x[3] = {1,0,4}, y[3] = {2,4,1};
	fill_polygon(img,3,x,y,white);
	unsigned char* where = img->data;
	for(int i = 0; i<5; i++) {
		for(int j = 0; j<5; j++) {
			for(int k = 0; k<3; k++) {
				printf("%d\t",*where);
				where++;
			}
		}
		printf("\n");
	}
	return 0;
}
