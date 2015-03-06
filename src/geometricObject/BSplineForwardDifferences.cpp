/*
 * BSplineForwardDifferences.cpp
 *
 *  Created on: 02/05/2013
 *      Author: gabriel
 */

#include "geometricObject/BSplineForwardDifferences.h"

BSpline_ForwardDifferences::BSpline_ForwardDifferences(VectorList vl, Label name, Color rgb) : GeometricShape(name, rgb) {
	if(vl.size() < 4){
		this->vectorList = vl;
		this->WvectorList = vl;
	}else{
		calculateCurve(vl);
	}
}

BSpline_ForwardDifferences::~BSpline_ForwardDifferences() {
	// TODO Auto-generated destructor stub
}

VectorList BSpline_ForwardDifferences::getVectorList() const{
	return this->vectorList;
}

void BSpline_ForwardDifferences::setVectorList( const VectorList vl)
{
	this->vectorList = vl;
}

GeometricShape::Type BSpline_ForwardDifferences::getType() const{
	return GeometricShape::B_SPLINE_FORWARD_DIFFERENCES;
}


VectorList BSpline_ForwardDifferences::getWVectorList() const{
	return this->WvectorList;
}

void BSpline_ForwardDifferences::setWVectorList(const VectorList vl) {

	this->WvectorList = vl;
}

void BSpline_ForwardDifferences::calculateCurve(VectorList vl){
	double a = 0.1;
	Matrix bs = initBS();
	Matrix e = initE(a);

	for (int i = 0; i < vl.size() -3; i++){

		Matrix gx = Matrix(4,1);
		Matrix gy = Matrix(4,1);

		gx(0,0) = vl.at(i).getX();
		gy(0,0) = vl.at(i).getY();

		gx(1,0) = vl.at(i+1).getX();
		gy(1,0) = vl.at(i+1).getY();

		gx(2,0) = vl.at(i+2).getX();
		gy(2,0) = vl.at(i+2).getY();

		gx(3,0) = vl.at(i+3).getX();
		gy(3,0) = vl.at(i+3).getY();

		Matrix cx = bs * gx;
		Matrix cy = bs * gy;

		calcFwdDiff(1/a, e * cx, e * cy);

	}

	this->vectorList = VectorList(WvectorList);

}

Matrix BSpline_ForwardDifferences::initBS() {

	Matrix bs = Matrix(4,4);

	bs(0,0) = -(1.0/6.0);
	bs(0,1) = 1.0/2.0;
	bs(0,2) = -(1.0/2.0);
	bs(0,3) = 1.0/6.0;

	bs(1,0) = 1.0/2.0;
	bs(1,1) = -1;
	bs(1,2) = 1.0/2.0;
	bs(1,3) = 0;

	bs(2,0) = -(1.0/2.0);
	bs(2,1) = 0;
	bs(2,2) = 1.0/2.0;
	bs(2,3) = 0;

	bs(3,0) = 1.0/6.0;
	bs(3,1) = 2.0/3.0;
	bs(3,2) = 1.0/6.0;
	bs(3,3) = 0;

	return bs;
}

Matrix BSpline_ForwardDifferences::initE(double a) {

	Matrix e = Matrix(4,4);

	e(0,0) = 0;
	e(0,1) = 0;
	e(0,2) = 0;
	e(0,3) = 1;

	e(1,0) = a*a*a;
	e(1,1) = a*a;
	e(1,2) = a;
	e(1,3) = 0;

	e(2,0) = 6*(a*a*a);
	e(2,1) = 2*(a*a);
	e(2,2) = 0;
	e(2,3) = 0;

	e(3,0) = 6*(a*a*a);
	e(3,1) = 0;
	e(3,2) = 0;
	e(3,3) = 0;

	return e;
}

FaceList BSpline_ForwardDifferences::getFaceList() const {
	FaceList list;
	return list;
}

void BSpline_ForwardDifferences::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito//nada a ser feito
}

void BSpline_ForwardDifferences::calcFwdDiff(int n, Matrix cx, Matrix cy) {

	this->WvectorList.append(Vec3D(cx(0,0),cy(0,0)));
	for (int i = 0; i < n; i++){
		cx(0,0) = cx(0,0) + cx(1,0);
		cy(0,0) = cy(0,0) + cy(1,0);

		cx(1,0) = cx(1,0) + cx(2,0);
		cy(1,0) = cy(1,0) + cy(2,0);

		cx(2,0) = cx(2,0) + cx(3,0);
		cy(2,0) = cy(2,0) + cy(3,0);

		this->WvectorList.append(Vec3D(cx(0,0),cy(0,0)));
	}

}
