/*
 * Window.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#include "Window.h"

Window::Window(Label name, Color rgb) : GeometricShape(name, rgb){
//TODO
	this->windowCenter = Vec3D(0, 0, -500);
	this->height = 600;
	this->width = 600;
	this->vUp = Vec3D::axis_Y;
	this->vPN = Vec3D::axis_Z;
}

Window::Window(Vec3D center, axis width, axis height, Label name, Color rgb) : GeometricShape(name, rgb) {
	this->windowCenter = center;
	this->width = width;
	this->height = height;

	this->vUp = Vec3D::axis_Y;
	this->vPN = Vec3D::axis_Z;
}

Window::~Window() {}

axis Window::getXMin() const{
	return  - (width/2.0);
}

axis Window::getYMin() const{
	return  - (height/2.0);
}

axis Window::getXMax() const{
	return  + (width/2.0);
}

axis Window::getYMax() const{
	return  + (height/2.0);
}

VectorList Window::getVectorList() const
{
	VectorList list = VectorList();
	list.append(this->windowCenter);
	return list;
}

GeometricShape::Type Window::getType() const
{
	return GeometricShape::WINDOW;
}

Angle Window::getWindowAngle() {
	axis dotProd = vUp.dotProduct(Vec3D::axis_Y);
	axis norm1 = vUp.length();
	axis norm2 = Vec3D::axis_Y.length();

	return GeometricShape::angle(acos(dotProd / (norm1 * norm2)));
}

void Window::setVectorList( const VectorList vl) {
	if(vl.size() == 1){
		this->windowCenter = Vec3D(vl.at(0));
	}
	else{
		this->windowCenter = Vec3D(250, 250);
	}
}

VectorList Window::getWVectorList() const
{
	VectorList list = VectorList();
	list.append(this->windowCenter);
	return list;
}

void Window::setWVectorList( const VectorList vl) {
	if(vl.size() == 1){
		this->windowCenter = Vec3D(vl.at(0));
	}
	else{
		this->windowCenter = Vec3D(250, 250);
	}

}

void Window::rotate(Angle angle, Vec3D vec, Vec3D axis) {
			Point wc = Point(this->windowCenter);
			Point vpnNew = Point(this->vPN);
			Point vupNew = Point(this->vUp);

			wc.rotate(angle, vec, axis);
			vpnNew.rotate(angle, Vec3D::Origin, axis);
			vupNew.rotate(angle, Vec3D::Origin, axis);

			this->windowCenter = wc.getWvector();
			this->vUp = vupNew.getWvector();
			this->vPN = vpnNew.getWvector();

			this->vUp = this->vUp.unitVector();
			this->vPN = this->vPN.unitVector();
}

void Window::translate(Vec3D vector)
{
	Point p = Point(vector);

	p.rotate(GeometricShape::angleDiffFromAxys(this->vUp, Vec3D::axis_Y), Vec3D(0,0,0), Vec3D(0,0,1));
	GeometricShape::translate(p.getVector());

}

FaceList Window::getFaceList() const {
	FaceList list;
	return list;
}

Vec3D Window::center() {
	return this->windowCenter;
}

void Window::scale(Vec3D factor) {
	this->width *= factor.getX();
	this->height *= factor.getY();
}

void Window::setFaceList(const FaceList list) {
	list.isEmpty(); //nada a ser feito
}
