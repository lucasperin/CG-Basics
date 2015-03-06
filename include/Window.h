/*
 * Window.h
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */
#ifndef COORDINATE_PLANE_H_
#define COORDINATE_PLANE_H_

#include "GeometricShape.h"
#include "Point.h"
#include <iostream>

/*
 * Origem da Window fica no canto INFERIOR esquerdo
 * Origem do ViewPort fica no canto SUPERIOR esquerdo
 */
class Window : public GeometricShape{
public:
	Window(Label name, Color rgb = QColor());
	Window(Vec3D center, axis width, axis height, Label name,  Color rgb = QColor());
	virtual ~Window();

	axis getXMin() const;

	axis getYMin() const;

	axis getXMax() const;

	axis getYMax() const;

	Angle getWindowAngle();

	/* from GeometricShape */
	virtual VectorList getVectorList() const;
	virtual GeometricShape::Type getType() const;
	virtual void setVectorList( const VectorList vl);
	virtual VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);

	virtual void rotate(Angle angle, Vec3D vecStr, Vec3D vecEnd);
	void translate(Vec3D vector);
	virtual Vec3D center();
	virtual void scale(Vec3D factor);

	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList list) ;

	Vec3D vUp;
	Vec3D vPN;

private:
	axis width;
	axis height;
	Vec3D windowCenter;

};

#endif /* COORDINATE_PLANE_H_ */
