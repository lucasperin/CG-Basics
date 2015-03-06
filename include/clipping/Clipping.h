/*
 * Clipping.h
 *
 *  Created on: 18/04/2013
 *      Author: gabriel
 */

#ifndef CLIPPING_H_
#define CLIPPING_H_

#include "geometricObject/Point.h"
#include "geometricObject/WireFrame.h"
#include "geometricObject/Line.h"
#include "geometricObject/BezierCurve2D.h"
#include "Window.h"

class Clipping {

public:
	Clipping(axis xMin, axis yMin, axis xMax, axis yMax){
		coordinates.setWvecStr(Vec3D(xMin, yMin));
		coordinates.setWvecEnd(Vec3D(xMax, yMax));
	};

	virtual ~Clipping(){};

	virtual GeometricShape* clip(GeometricShape* gs)
	{
		switch(gs->getType())
		{
		case GeometricShape::POINT:
			return this->clip((Point*)gs);
		case GeometricShape::LINE:
			return this->clip((Line*)gs);
		case GeometricShape::WIREFRAME:
			return this->clip((WireFrame*)gs);
		case GeometricShape::WINDOW:
			return gs; //TODO clipagem da window caso ela seja desenhada para outras cameras.
		case GeometricShape::BEZIER_CURVE_2D:
			return this->clip((BezierCurve2D*)gs);
		case GeometricShape::B_SPLINE_FORWARD_DIFFERENCES:
			return this->clip((BezierCurve2D*)gs);
		case GeometricShape::OBJECT3D:
			return gs;

		}
		return 0;
	}

	virtual void setCoordinates(axis xMin, axis yMin, axis xMax, axis yMax, Vec3D sizeFactor){
		coordinates.setWvecStr(Vec3D(xMin, yMin));
		coordinates.setWvecEnd(Vec3D(xMax, yMax));
		coordinates.scale(sizeFactor);
	}

	virtual void setWindowCoordinates(Window* wd, Vec3D sizeFactor){
		coordinates.setWvecStr(Vec3D(wd->getXMin(), wd->getYMin()));
		coordinates.setWvecEnd(Vec3D(wd->getXMax(), wd->getYMax()));
		coordinates.scale(sizeFactor);
	}

	axis getXMax() const {
		return coordinates.getWvecEnd().getX() ;
	}

	axis getXMin() const {
		return coordinates.getWvecStr().getX() ;
	}

	axis getYMax() const {
		return coordinates.getWvecEnd().getY() ;
	}

	axis getYMin() const {
		return coordinates.getWvecStr().getY();
	}

protected:
	Line coordinates;

	virtual Point* clip(Point* p) = 0;
	virtual Line* clip(Line* l) = 0;
	virtual WireFrame* clip(WireFrame* wf) = 0;
	virtual BezierCurve2D* clip(BezierCurve2D* bc) = 0;

};


#endif /* CLIPPING_H_ */
