/*
 * CohenSutherlandClipping.h
 *
 *  Created on: 18/04/2013
 *      Author: gabriel
 */

#ifndef COHENSUTHERLANDCLIPPING_H_
#define COHENSUTHERLANDCLIPPING_H_

#include "Clipping.h"
#include "Vec3D.h"
#include "Window.h"

class CohenSutherlandClipping: public Clipping {

	public:
	CohenSutherlandClipping(axis xMin, axis yMin, axis xMax, axis yMax);
	CohenSutherlandClipping(Window* window);
	virtual ~CohenSutherlandClipping();

protected:
	enum Quadrant
	{
		INSIDE = 0x0,
		LEFT = 0x1,
		RIGHT =0x2,
		DOWN = 0x4,
		UP = 0x8,
		UP_LEFT = UP | LEFT,
		UP_RIGHT = UP | RIGHT,
		DOWN_LEFT = DOWN | LEFT,
		DOWN_RIGHT = DOWN | RIGHT
	};

	CohenSutherlandClipping::Quadrant getQuadrant(Vec3D v);
	Vec3D clipLineVector(Vec3D v, axis m, CohenSutherlandClipping::Quadrant quadrant);
	axis calculateM(Vec3D start, Vec3D end);
	axis calculateM(Line* l);
	Vec3D calculateIntersection(Vec3D v1, Vec3D v2, int edge);

	virtual Point* clip(Point* p);
	virtual Line* clip(Line* l);
	virtual WireFrame* clip(WireFrame* wf);
	virtual BezierCurve2D* clip(BezierCurve2D* bc);


};

#endif /* COHENSUTHERLANDCLIPPING_H_ */
