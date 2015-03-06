/*
 * Line.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#include "Line.h"

Line::Line(Vec3D vecStr, Vec3D vecEnd, Label name, Color rgb) : GeometricShape(name, rgb){
	setVecStr(vecStr);
	setVecEnd(vecEnd);
	setWvecStr(vecStr);
	setWvecEnd(vecEnd);
}

Line::Line(Vec3D vecStr, Vec3D vecEnd) : GeometricShape(){
	setVecStr(vecStr);
	setVecEnd(vecEnd);
	setWvecStr(vecStr);
	setWvecEnd(vecEnd);
}

Line::~Line() {}

const Vec3D& Line::getVecEnd() const {
	return vecEnd;
}

void Line::setVecEnd(const Vec3D& vecEnd) {
	this->vecEnd = vecEnd;
}

const Vec3D& Line::getVecStr() const {
	return vecStr;
}

void Line::setVecStr(const Vec3D& vecStr) {
	this->vecStr = vecStr;
}

VectorList Line::getVectorList() const{
	VectorList list = VectorList();
	list.append(getVecStr());
	list.append(getVecEnd());
	return list;
}

void Line::setVectorList( const VectorList vl)
{
	if(vl.size() >= 2)
	{
		this->vecStr = Vec3D(vl.at(0));
		this->vecEnd = Vec3D(vl.at(1));
	}
}

GeometricShape::Type Line::getType() const {
	return GeometricShape::LINE;
}

const Vec3D& Line::getWvecEnd() const {
	return WvecEnd;
}

void Line::setWvecEnd(const Vec3D& wvecEnd) {
	WvecEnd = wvecEnd;
}

const Vec3D& Line::getWvecStr() const {
	return WvecStr;
}

void Line::setWvecStr(const Vec3D& wvecStr) {
	WvecStr = wvecStr;
}

VectorList Line::getWVectorList() const {
	VectorList list =VectorList();
	list.append(getWvecStr());
	list.append(getWvecEnd());
	return list;
}

void Line::setWVectorList( const VectorList vl) {
	if(vl.size() >= 2)
	{
		this->WvecStr = Vec3D(vl.at(0));
		this->WvecEnd = Vec3D(vl.at(1));
	}
}

FaceList Line::getFaceList() const {
	FaceList list;
	return list;
}

void Line::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito
}
