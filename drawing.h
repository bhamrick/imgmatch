#include<vector>
#include<algorithm>

struct color {
	double r, g, b, a;
};

void draw_polygon(std::vector< std::pair<int,int> > v, color c, color* buf, int width, int height);
