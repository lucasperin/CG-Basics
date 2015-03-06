#include "BezierCurve2D.h"
#include <iostream>

BezierCurve2D::BezierCurve2D(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4) : GeometricShape() {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	setVectorList(calculateCurve(p1, p2, p3, p4));
	this->Wp1 = p1;
	this->Wp2 = p2;
	this->Wp3 = p3;
	this->Wp4 = p4;
	setWVectorList(calculateCurve(p1, p2, p3, p4));
}

BezierCurve2D::BezierCurve2D(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Label name, Color rgb) : GeometricShape(name, rgb){
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	setVectorList(calculateCurve(p1, p2, p3, p4));
	this->Wp1 = p1;
	this->Wp2 = p2;
	this->Wp3 = p3;
	this->Wp4 = p4;
	setWVectorList(calculateCurve(p1, p2, p3, p4));
}

VectorList BezierCurve2D::getVectorList() const {
	return this->vectorList;
}

void BezierCurve2D::setVectorList(const VectorList vl) {
	this->vectorList = vl;
}

GeometricShape::Type BezierCurve2D::getType() const {
	return GeometricShape::BEZIER_CURVE_2D;
}

VectorList BezierCurve2D::getWVectorList() const {
	return this->WvectorList;
}

void BezierCurve2D::setWVectorList(const VectorList vl) {
	this->WvectorList = vl;
}

FaceList BezierCurve2D::getFaceList() const {
	FaceList list;
	return list;
}

void BezierCurve2D::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito//nada a ser feito
}

VectorList BezierCurve2D::calculateCurve(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4) {
	VectorList vl;
	Vec3D p;

	Matrix Mb(4,4);
	Matrix Gbx(4,1);
	Matrix Gby(4,1);
	Matrix Gbz(4,1);

	Mb(0,0) = -1;
	Mb(0,1) = 3;
	Mb(0,2) = -3;
	Mb(0,3) = 1;
	Mb(1,0) = 3;
	Mb(1,1) = -6;
	Mb(1,2) = 3;
	Mb(2,0) = -3;
	Mb(2,1) = 3;
	Mb(3,0) = 1;

	Gbx(0,0) = p1.getX();
	Gbx(1,0) = p2.getX();
	Gbx(2,0) = p3.getX();
	Gbx(3,0) = p4.getX();

	Gby(0,0) = p1.getY();
	Gby(1,0) = p2.getY();
	Gby(2,0) = p3.getY();
	Gby(3,0) = p4.getY();

	Gbz(0,0) = p1.getZ();
	Gbz(1,0) = p2.getZ();
	Gbz(2,0) = p3.getZ();
	Gbz(3,0) = p4.getZ();

	Matrix t(1,4);
	for(double i = 0; i < 1; i += 0.01)
	{
		t(0,0) = pow(i, 3);
		t(0,1) = pow(i, 2);
		t(0,2) = pow(i, 1);
		t(0,3) = 1;

		Matrix x = t * Mb * Gbx;
		Matrix y = t * Mb * Gby;
		Matrix z = t * Mb * Gbz;

		p = Vec3D(x(0,0), y(0,0), z(0,0));
		vl.append(p);
	}
	vl.append(p4);
	return vl;
}
