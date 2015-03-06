/*
 * Vec3.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#include "Vec3D.h"

Vec3D Vec3D::Origin = Vec3D(0,0,0);
Vec3D Vec3D::axis_X = Vec3D(1,0,0);
Vec3D Vec3D::axis_Y = Vec3D(0,1,0);
Vec3D Vec3D::axis_Z = Vec3D(0,0,1);

Vec3D::Vec3D() {
	setX(0);
	setY(0);
	setZ(0);
	setW(1);
}

Vec3D::Vec3D(axis x, axis y, axis z, axis w){
	setX(x);
	setY(y);
	setZ(z);
	setW(w);
}

Vec3D::~Vec3D() {}



Vec3D::Vec3D(const Vec3D& v) {
	setX(v.getX());
	setY(v.getY());
	setZ(v.getZ());
	setW(v.getW());
}

Vec3D& Vec3D::operator =(const Vec3D& v) {
	setX(v.getX());
	setY(v.getY());
	setZ(v.getZ());
	setW(v.getW());
	return *this;
}

axis Vec3D::getX() const{
	return this->x;
}

void Vec3D::setX(axis x){
	this->x = x;
}

axis Vec3D::getY() const{
	return this->y;
}

void Vec3D::setY(axis y){
	this->y = y;
}

axis Vec3D::getZ() const{
	return this->z;
}

void Vec3D::setZ(axis z){
	this->z = z;
}

axis Vec3D::getW() const {
	return w;
}

void Vec3D::setW(axis w) {
	this->w = w;
}

/*
 * length == norm == magnitude
 */
double Vec3D::length() const {
	double ret = (getX()*getX()) + (getY()*getY()) + (getZ()*getZ());
	return sqrt(ret);
}

Vec3D Vec3D::crossProduct(Vec3D& v) const {

	axis c1 = (getY()*v.getZ()) - (getZ()*v.getY());
	axis c2 = (getZ()*v.getX()) - (getX()*v.getZ());
	axis c3 = (getX()*v.getY()) - (getY()*v.getX());
	return Vec3D(c1,c2,c3);
}

Vec3D Vec3D::unitVector() {

	const double length = this->length();
	if(length == 0.0)
		return *this;
	return Vec3D(getX()/length, getY()/length, getZ()/ length);
}

Vec3D Vec3D::operator *(const Vec3D& v) {
	Vec3D ret;
	ret.setX(getX() * v.getX());
	ret.setY(getY() * v.getY());
	ret.setZ(getZ() * v.getZ());
	return ret;
}

Vec3D Vec3D::operator +(const Vec3D& v) {
	Vec3D ret;
	ret.setX(getX() + v.getX());
	ret.setY(getY() + v.getY());
	ret.setZ(getZ() + v.getZ());
	return ret;
}
Vec3D Vec3D::operator -(const Vec3D& v) {
	return *this + v.inverse();
}

axis Vec3D::dotProduct(Vec3D& v) const {
	return (getX()*v.getX()) + (getY() * v.getY()) + (getZ() * v.getZ());
}

Vec3D Vec3D::inverse() const
{
	return Vec3D(-this->x, -this->y, -this->z);
}

void Vec3D::print(QString s){
	std::cout << s.toStdString() << " - [" << this->getX() << "," << this->getY() << "," << this->getZ() << "]" << std::endl;
}
