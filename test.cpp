#include<cstdio>
#include<cairo.h>
#include<cstdlib>

using namespace std;

int main(int argc, char** argv) {
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 200, 200);
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_paint(cr);
	cairo_set_source_rgba(cr,1.0,1.0,1.0,1.0);
	cairo_new_path(cr);
	cairo_move_to(cr,50,50);
	cairo_line_to(cr,200,100);
	cairo_line_to(cr,150,23);
	cairo_close_path(cr);
	cairo_fill(cr);
	cairo_destroy(cr);
	unsigned char* data = cairo_image_surface_get_data(surface);
	for(int i = 0; i<200; i++) {
		for(int j = 0; j<200; j++) {
			for(int k = 0; k<4; k++) {
				printf("%d ",*data);
				data++;
			}
		}
		printf("\n");
	}
	cairo_surface_write_to_png(surface,"triangle.png");
	cairo_surface_destroy(surface);
	return 0;
}
