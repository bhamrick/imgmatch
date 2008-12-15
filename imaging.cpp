#include<imaging.h>

image_t* init(int w, int h) {
	image_t * ans = malloc(sizeof(image_t));
	ans->w = w;
	ans->h = h;
	ans->data = malloc(3*w*h*sizeof(unsigned char));
	memset(ans->data,3*w*h*sizeof(unsigned char),0);
}

bitmap_t* init_bm(int w, int h) {
	bitmap_t *ans = malloc(sizeof(bitmap_t));
	ans->w = w;
	ans->h = h;
	ans->data = malloc(w*h*sizeof(bool));
	memset(ans->data,w*h*sizeof(bool),0);
}

void destroy(bitmap_t* bm) {
	free(bm->data);
	free(bm);
}

void destroy(image_t* img) {
	free(img->data);
	free(img);
}

bool same_side(int x1, int y1, int x2, int y2, int xa, int ya, int xb, int yb) {
	//returns true iff (xa,ya) and (xb,yb) are on the same side of (x1,y1)--(x2,y2)
	if(x1 == x2) {
		return (xa-x1)*(xb-x1)>=0;
	}
	return (
}

void xor_triangle(bitmap_t* bm, int* x, int* y) {
	int xmin = min(x[0],min(x[1],x[2])), xmax = max(x[0],max(x[1],x[2])),
	    ymin = min(y[0],min(y[1],y[2])), ymax = max(y[0],max(y[1],y[2]));
	for(int x = xmin; x <= xmax; x++) {
		for(int y = ymin, y<=ymax; y++) {
			if(same_side(x[0],y[0],x[1],y[1],x,y,x[2],y[2]) &&
			   same_side(x[1],y[1],x[2],y[2],x,y,x[0],y[0]) &&
			   same_side(x[2],y[2],x[0],y[0],x,y,x[1],y[1])) {
				bm->data[y*w+x] ^= 1;
			}
		}
	}
}

void fill_polygon(image_t* img, int n, int* x, int* y, color_t c) {
	bitmap_t* bm = init_bm(img->w,img->h);
	int xmin = img->w, xmax = 0, ymin = img->h, ymax = 0;
	int tx[3], ty[3];
	for(int i = 0; i<n-1; i++) {
		xmin = min(xmin,x[i]);
		xmax = max(xmax,x[i]);
		ymin = min(ymin,y[i]);
		ymax = max(ymax,y[i]);
		tx[0] = x[0];
		ty[0] = y[0];
		tx[1] = x[i];
		ty[1] = y[i];
		tx[2] = x[i+1];
		ty[2] = y[i+1];
		xor_triangle(bm,tx,ty); 
	}
	xmin = min(xmin,x[n-1]);
	xmax = max(xmax,x[n-1]);
	ymin = min(ymin,y[n-1]);
	ymax = max(ymax,y[n-1]);
	for(int ix = xmin; ix <= xmax; ix++) {
		for(int iy = ymin; iy <= ymax; iy++) {
			if(bm->data[iy*w+ix]) {
				img->data[3*(iy*w+ix)] = c.a*c.r + (1-c.a)*img->data[3*(iy*w + ix)];
				img->data[3*(iy*w+ix)+1] = c.a*c.g + (1-c.a)*img->data[3*(iy*w + ix)+1];
				img->data[3*(iy*w+ix)+2] = c.a*c.b + (1-c.a)*img->data[3*(iy*w + ix)+2];
			}
		}
	}
}
