/*
 * WireFrame.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: perin
 */

#include "WireFrame.h"

WireFrame::WireFrame(VectorList list, Label name, Color rgb, bool brushed) : GeometricShape(name, rgb){
	this->vectorList = VectorList(list);
	this->WvectorList = VectorList(list);
	this->brushed = brushed;
}

WireFrame::WireFrame(VectorList list, bool brushed) : GeometricShape(){
	this->vectorList = VectorList(list);
	this->WvectorList = VectorList(list);
	this->brushed = brushed;
}

WireFrame::~WireFrame() {
}

VectorList WireFrame::getVectorList() const{
	return this->vectorList;
}

void WireFrame::setVectorList( const VectorList vl)
{
	this->vectorList = vl;
}

GeometricShape::Type WireFrame::getType() const{
	return GeometricShape::WIREFRAME;
}


VectorList WireFrame::getWVectorList() const{
	return this->WvectorList;
}

void WireFrame::setWVectorList(const VectorList vl) {

	this->WvectorList = vl;
}

void WireFrame::setBrushed(bool brushed)
{
	this->brushed = brushed;
}

FaceList WireFrame::getFaceList() const {
	FaceList list;
	return list;
}

void WireFrame::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito//nada a ser feito
}

bool WireFrame::getBrushed()
{
	return this->brushed;
}
