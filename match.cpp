#include<cstdio>
#include<cairo.h>
#include<cstdlib>

using namespace std;

double diff(unsigned char* data1, unsigned char* data2, int size) {
	double ans = 0.0;
	for(int i = 0; i<size; i++) {
		ans += 0.5*(*data1-*data2)*(*data1-*data2);
		data1++;
		data2++;
	}
	return ans;
}

int main(int argc, char** argv) {
	if(argc == 1) {
		printf("Usage: %s filename\n",argv[0]);
		return 0;
	}
	cairo_surface_t *target = cairo_image_surface_create_from_png(argv[1]);
	unsigned char* tdata = cairo_image_surface_get_data(target);
	
}
