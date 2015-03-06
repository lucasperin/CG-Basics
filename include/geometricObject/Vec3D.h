/*
 * Vec3.h
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>
#include <iostream>
#include <QString>

typedef double axis;

class Vec3D {
public:

	Vec3D();
	Vec3D(axis x, axis y, axis z = 0, axis w = 1);
	virtual ~Vec3D();
	Vec3D(const Vec3D &v);
	Vec3D& operator=(const Vec3D &v);
	Vec3D operator*(const Vec3D &v);
	Vec3D operator+(const Vec3D &v);
	Vec3D operator-(const Vec3D &v);

	/* getters & setters */
	axis getX() const;
	void setX(axis x);

	axis getY() const;
	void setY(axis y);

	axis getZ() const;
	void setZ(axis z);

	axis getW() const;
	void setW(axis w);

	double length() const;

	Vec3D unitVector();

	Vec3D crossProduct(Vec3D& v) const;

	axis dotProduct(Vec3D& v) const;

	Vec3D inverse() const;

	void print(QString s);

	static Vec3D Origin;
	static Vec3D axis_X;
	static Vec3D axis_Y;
	static Vec3D axis_Z;

protected:
	axis x;
	axis y;
	axis z;
	axis w;
};

#endif /* VEC3_H_ */
