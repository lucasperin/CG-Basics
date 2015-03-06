/*
 * LiangBarskyClipping.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: lucasperin
 */

#include "LiangBarskyClipping.h"

LiangBarskyClipping::LiangBarskyClipping(axis xMin, axis yMin, axis xMax, axis yMax) : CohenSutherlandClipping(xMin, yMin, xMax, yMax) {
	// TODO Auto-generated constructor stub

}

LiangBarskyClipping::LiangBarskyClipping(Window* window) : CohenSutherlandClipping(window) {
	// TODO Auto-generated constructor stub

}

LiangBarskyClipping::~LiangBarskyClipping() {
	// TODO Auto-generated destructor stub
}

Line* LiangBarskyClipping::clip(Line* l)
{
	Vec3D vec1 = l->getVecEnd();
	Vec3D vec2 = l->getVecStr();

	axis x1,y1,x2,y2;
	x1 = vec1.getX();
	y1 = vec1.getY();
	x2 = vec2.getX();
	y2 = vec2.getY();

    axis u1 = 0.0;
    axis u2 = 1.0;
    axis xdelta = x2-x1;
    axis ydelta = y2-y1;
    axis p,q,r;

	for (int edge = 0; edge < 4; edge++) {
		if (edge == 0) {
			p = -xdelta;
			q = -(this->getXMin() - x1);
		}
		if (edge == 1) {
			p = xdelta;
			q = (this->getXMax() - x1);
		}
		if (edge == 2) {
			p = -ydelta;
			q = -(this->getYMin() - y1);
		}
		if (edge == 3) {
			p = ydelta;
			q = (this->getYMax() - y1);
		}
		r = q / p;
		if (p == 0 && q < 0)
			return 0;

		if (p < 0) {
			if (r > u2)
				return 0;
			else
				if (r > u1)
					u1 = r;
		} else if (p > 0) {
			if (r < u1)
				return 0;
			else if (r < u2)
				u2 = r;
		}
	}

    axis x1clipped = x1 + u1*xdelta;
    axis y1clipped = y1 + u1*ydelta;
    axis x2clipped = x1 + u2*xdelta;
    axis y2clipped = y1 + u2*ydelta;

	vec1.setX(x1clipped);
	vec1.setY(y1clipped);
	l->setVecEnd(vec1);
	vec2.setX(x2clipped);
	vec2.setY(y2clipped);
	l->setVecStr(vec2);

	return l;
}
