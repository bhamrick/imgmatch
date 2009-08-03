#include"drawing.h"

using namespace std;

void draw_polygon(vector< pair<int,int> > v, color c, color* buf, int width, int height) {
	sort(v.begin(), v.end());
	vector< pair<int,int> > lenv, uenv;
	for(int i = 0; i<v.size(); i++) {
		if(lenv.size() == 0 || lenv[lenv.size()-1].first != v[i].first) {
			while(lenv.size() > 1 && (lenv[lenv.size()-1].second-lenv[lenv.size()-2].second)*(v[i].first-lenv[lenv.size()-2].first) >= (v[i].second-lenv[lenv.size()-2].second)*(lenv[lenv.size()-1].first-lenv[lenv.size()-2].first)) lenv.pop_back();
			lenv.push_back(v[i]);
		}
		while(uenv.size() > 0 && uenv[uenv.size()-1].first == v[i].first) uenv.pop_back();
		while(uenv.size() > 1 && (uenv[uenv.size()-1].second-uenv[uenv.size()-2].second)*(v[i].first-uenv[uenv.size()-2].first) <= (v[i].second-uenv[uenv.size()-2].second)*(uenv[uenv.size()-1].first-uenv[uenv.size()-2].first)) uenv.pop_back();
		uenv.push_back(v[i]);
	}
	int i1 = 0, i2 = 0;
	for(int x = v[0].first; x <= v[v.size()-1].first; x++) {
		while(x > lenv[i1+1].first) i1++;
		while(x > uenv[i2+1].first) i2++;
		for(int y = lenv[i1].second + (lenv[i1+1].second-lenv[i1].second)*(x-lenv[i1].first)/(lenv[i1+1].first-lenv[i1].first), off = (x*height) + y; y <= uenv[i2].second + (double)(uenv[i2+1].second-uenv[i2].second)*(x-uenv[i2].first)/(uenv[i2+1].first-uenv[i2].first) + 0.5; y++, off++) {
			buf[off].r = buf[off].r*c.a + c.r*(1.0-c.a);
			buf[off].g = buf[off].g*c.a + c.g*(1.0-c.a);
			buf[off].b = buf[off].b*c.a + c.b*(1.0-c.a);
		}
	}
}
