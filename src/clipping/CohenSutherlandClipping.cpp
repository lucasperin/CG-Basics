/*
 * CohenSutherlandClipping.cpp
 *
 *  Created on: 18/04/2013
 *      Author: gabriel
 */

#include "CohenSutherlandClipping.h"

CohenSutherlandClipping::CohenSutherlandClipping(axis xMin, axis yMin, axis xMax, axis yMax) :
										Clipping(xMin, yMin, xMax, yMax){
}

CohenSutherlandClipping::CohenSutherlandClipping(Window* window) :
						Clipping(window->getXMin(), window->getYMin(), window->getXMax(), window->getYMax()){
}

CohenSutherlandClipping::~CohenSutherlandClipping() {
}

Point* CohenSutherlandClipping::clip(Point* p) {
	if(this->getQuadrant(p->getVector()) == CohenSutherlandClipping::INSIDE){
		return p;
	}
	return 0;
}

Line* CohenSutherlandClipping::clip(Line* l) {
	int quadrant_v1 = getQuadrant(l->getVecStr());
	int quadrant_v2 = getQuadrant(l->getVecEnd());

	//Linha totalmente fora
	if((quadrant_v1 & quadrant_v2) != 0){
		return 0;
	}

	//linha totalmente dentro
	if((quadrant_v1 | quadrant_v2) == 0){
		return l;
	}

	axis m = calculateM(l->getVecStr(), l->getVecEnd());
	l->setVecStr(this->clipLineVector(l->getVecStr(), m, (CohenSutherlandClipping::Quadrant)quadrant_v1));
	l->setVecEnd(this->clipLineVector(l->getVecEnd(), m, (CohenSutherlandClipping::Quadrant)quadrant_v2));

	if( (getQuadrant(l->getVecStr()) | getQuadrant(l->getVecEnd())) != 0){
		return 0;
	}
	return l;
}

WireFrame* CohenSutherlandClipping::clip(WireFrame* wf) {
	VectorList vectors = wf->getVectorList();
	Vec3D a, b;


	bool needToClip = false;
	for(int i = 0; i < vectors.size(); i++){
		if( i < vectors.size() - 1){
			a = vectors.at(i);
			b = vectors.at(i+1);
		} else {
			a = vectors.at(vectors.size() -1);
			b = vectors.at(0);
		}

		if( (getQuadrant(a) & getQuadrant(b)) == 0){
			needToClip = true;
			break;
		}
	}
	if( ! needToClip ){
		return 0;
	}


	VectorList temp;
	//Cortando por borda
	for(int edge = 1; edge <= 8; edge*=2){
		temp.clear();
		for(int i = 0; i < vectors.size(); i++){
			if( i < vectors.size() - 1){
				a = vectors.at(i);
				b = vectors.at(i+1);
			} else {
				a = vectors.at(vectors.size() -1);
				b = vectors.at(0);
			}

			if( (getQuadrant(a) & edge) == 0){
				// a esta dentro da area de clipping
				if((getQuadrant(b) & edge) == 0){
					temp.append(b);
				}else{
					temp.append(calculateIntersection(a, b, edge));
				}
			} else if((getQuadrant(b) & edge) == 0){
				// a esta fodra da area de cliping mas b esta dentro
				temp.append(calculateIntersection(a, b, edge));
				temp.append(b);
			}
		}

		vectors.clear();
		vectors.append(temp);
	}
	wf->setVectorList(vectors);

	return wf;
}

BezierCurve2D* CohenSutherlandClipping::clip(BezierCurve2D* bc)
{

	VectorList vectors = bc->getVectorList();
	Vec3D a, b;


	bool needToClip = false;
	for(int i = 0; i < vectors.size()-1; i++){
		a = vectors.at(i);
		b = vectors.at(i+1);

		if( (getQuadrant(a) & getQuadrant(b)) == 0){
			needToClip = true;
			break;
		}
	}
	if( ! needToClip ){
		return 0;
	}


	VectorList temp;
	//Cortando por borda
	for(int edge = 1; edge <= 8; edge*=2){
		temp.clear();
		for(int i = 0; i < vectors.size()-1; i++){
				a = vectors.at(i);
				b = vectors.at(i+1);

			if( (getQuadrant(a) & edge) == 0){
				temp.append(a);
				// a esta dentro da area de clipping
				if((getQuadrant(b) & edge) == 0){
					temp.append(b);
				}else{
					temp.append(calculateIntersection(a, b, edge));
				}
			} else if((getQuadrant(b) & edge) == 0){
				// a esta fora da area de cliping mas b esta dentro
				temp.append(calculateIntersection(a, b, edge));
				temp.append(b);
			}
		}

		vectors.clear();
		vectors.append(temp);
	}
	bc->setVectorList(vectors);

	return bc;

}


Vec3D CohenSutherlandClipping::calculateIntersection(Vec3D v1, Vec3D v2, int edge){
	return clipLineVector(v1, calculateM(v1, v2), (CohenSutherlandClipping::Quadrant)edge);
}

CohenSutherlandClipping::Quadrant CohenSutherlandClipping::getQuadrant(Vec3D v) {
	axis x = v.getX();
	axis y = v.getY();
	int quadrant = 0;

	if(x < this->getXMin()){
		quadrant += CohenSutherlandClipping::LEFT;
	}else if(x > this->getXMax()){
		quadrant += CohenSutherlandClipping::RIGHT;
	}

	if(y < this->getYMin()){
		quadrant += CohenSutherlandClipping::DOWN;
	}else if(y > this->getYMax()){
		quadrant += CohenSutherlandClipping::UP;
	}
	return (CohenSutherlandClipping::Quadrant)quadrant;
}

axis CohenSutherlandClipping::calculateM(Line* l){
	return (l->getVecEnd().getY() - l->getVecStr().getY())/(l->getVecEnd().getX() - l->getVecStr().getX());
}

axis CohenSutherlandClipping::calculateM(Vec3D start, Vec3D end){
	return (end.getY() - start.getY())/(end.getX() - start.getX());
}

Vec3D CohenSutherlandClipping::clipLineVector(Vec3D v, axis m, CohenSutherlandClipping::Quadrant quadrant){

	switch (quadrant) {
	case CohenSutherlandClipping::INSIDE:
		break;
	case CohenSutherlandClipping::LEFT:
		if (m != 0.0) {
			v.setY((m * (this->getXMin() - v.getX())) + v.getY());
		}
		v.setX(this->getXMin());
		break;
	case CohenSutherlandClipping::RIGHT:
		if (m != 0.0) {
			v.setY((m * (this->getXMax() - v.getX())) + v.getY());
		}
		v.setX(this->getXMax());
		break;
	case CohenSutherlandClipping::UP:
		if (m != 0.0) {
			v.setX(v.getX() + ((1 / m) * (this->getYMax() - v.getY())));
		}
		v.setY(this->getYMax());
		break;
	case CohenSutherlandClipping::DOWN:
		if (m != 0.0) {
			v.setX(v.getX() + ((1 / m) * (this->getYMin() - v.getY())));
		}
		v.setY((this->getYMin()));
		break;
	case CohenSutherlandClipping::UP_LEFT:

		if (m != 0.0) {
			axis tempY = (m * (this->getXMin() - v.getX())) + v.getY();
			if(getQuadrant(Vec3D(this->getXMin(), tempY, 0, 0)) != CohenSutherlandClipping::INSIDE){
				v.setX(v.getX() + ((1 / m) * (this->getYMax() - v.getY())));
				v.setY(this->getYMax());
			}else{
				v.setX(this->getXMin());
				v.setY(tempY);
			}
		}

		break;

	case CohenSutherlandClipping::UP_RIGHT:

		if (m != 0.0) {
			axis tempY = (m * (this->getXMax() - v.getX())) + v.getY();
			if(getQuadrant(Vec3D(this->getXMax(), tempY, 0, 0)) != CohenSutherlandClipping::INSIDE){
				v.setX(v.getX() + ((1 / m) * (this->getYMax() - v.getY())));
				v.setY(this->getYMax());
			}else{
				v.setX(this->getXMax());
				v.setY(tempY);
			}
		}

		break;

	case CohenSutherlandClipping::DOWN_LEFT:

		if (m != 0.0) {
			axis tempY = (m * (this->getXMin() - v.getX())) + v.getY();
			if(getQuadrant(Vec3D(this->getXMin(), tempY, 0, 0)) != CohenSutherlandClipping::INSIDE){
				v.setX(v.getX() + ((1 / m) * (this->getYMin() - v.getY())));
				v.setY((this->getYMin()));
			}else{
				v.setX(this->getXMin());
				v.setY(tempY);
			}
		}

		break;

	case CohenSutherlandClipping::DOWN_RIGHT:

		if (m != 0.0) {
			axis tempY = (m * (this->getXMax() - v.getX())) + v.getY();
			if(getQuadrant(Vec3D(this->getXMax(), tempY, 0, 0)) != CohenSutherlandClipping::INSIDE){
				v.setX(v.getX() + ((1 / m) * (this->getYMin() - v.getY())));
				v.setY((this->getYMin()));
			}else{
				v.setX(this->getXMax());
				v.setY(tempY);
			}
		}

		break;
	}
	return v;
}
