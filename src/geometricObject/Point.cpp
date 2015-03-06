/*
 * Point.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: lucasperin
 */

#include "Point.h"

Point::Point(Vec3D vector, Label name, Color rgb) : GeometricShape(name, rgb){
	setVector(vector);
	setWvector(vector);
}

Point::Point(Vec3D vector) : GeometricShape(){
	setVector(vector);
	setWvector(vector);
}

Point::~Point() {
}

const Vec3D& Point::getVector() const {
	return vector;
}

void Point::setVector(const Vec3D& vector) {
	this->vector = vector;
}

VectorList Point::getVectorList() const
{
	VectorList list = VectorList();
	list.append(getVector());

	return list;
}

void Point::setVectorList( const VectorList vl)
{
	if(vl.size() >= 1)
	{
		this->vector = Vec3D(vl.at(0));
	}
}

GeometricShape::Type Point::getType() const
{
	return GeometricShape::POINT;
}

const Vec3D& Point::getWvector() const {
	return Wvector;
}

void Point::setWvector(const Vec3D& wvector) {
	Wvector = wvector;
}

VectorList Point::getWVectorList() const
{
	VectorList list = VectorList();
	list.append(getWvector());

	return list;
}

void Point::setWVectorList( const VectorList vl) {
	if(vl.size() >= 1)
	{
		this->Wvector = Vec3D(vl.at(0));
	}
}

FaceList Point::getFaceList() const {
	FaceList list;
	return list;
}

void Point::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito//nada a ser feito
}
