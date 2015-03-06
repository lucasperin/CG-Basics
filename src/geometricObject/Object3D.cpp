/*
 * Object3D.cpp
 *
 *  Created on: May 13, 2013
 *      Author: lucasperin
 */

#include "Object3D.h"

Object3D::Object3D(VectorList vectorList, FaceList faceList, bool brushed) : GeometricShape(){
	this->vectorList = vectorList;
	this->WvectorList = vectorList;
	this->faceList = faceList;
	this->brushed = brushed;
}
Object3D::Object3D(QString file, Label name, Color rgb, bool brushed) : GeometricShape(name, rgb){
	this->brushed = brushed;
	buildObject(file);
}

Object3D::Object3D(VectorList vectorList, FaceList faceList, Label name,
		Color rgb, bool brushed) : GeometricShape(name, rgb){
	this->vectorList = vectorList;
	this->WvectorList = vectorList;
	this->faceList = faceList;
	this->brushed = brushed;
}

Object3D::~Object3D(){
}

bool Object3D::getBrushed(){
	return this->brushed;
}

VectorList Object3D::getVectorList() const {
	return this->vectorList;
}

void Object3D::setVectorList(const VectorList vl) {
	this->vectorList = vl;
}

GeometricShape::Type Object3D::getType() const {
	return GeometricShape::OBJECT3D;
}

VectorList Object3D::getWVectorList() const {
	return this->WvectorList;
}

void Object3D::setWVectorList(const VectorList vl) {
	this->WvectorList = vl;
}

FaceList Object3D::getFaceList() const {
	return this->faceList;
}

void Object3D::setFaceList(const FaceList list) {
	this->faceList = list;
}

void Object3D::buildObject(QString file) {
	QFile objectFile(file);
	if(objectFile.open(QIODevice::ReadOnly)){
		bool control = true;
		while(control){
			char buf[1024];
			qint64 sizeOfBuffer = objectFile.readLine(buf, sizeof(buf));
			if(sizeOfBuffer != -1){
				iterateLine(QString(buf));
			}else{
				control = false;
			}
		}
		objectFile.close();
		WvectorList = vectorList;
	}
}

void Object3D::iterateLine(QString line) {
	if(line.contains("v ")){

		line.remove(0, 2);
		QString aux = line.left(line.indexOf(" "));
		axis x = aux.toDouble();

		line.remove(0, line.indexOf(" ") +1);
		aux = line.left(line.indexOf(" "));
		axis y = aux.toDouble();

		line.remove(0, line.indexOf(" ") +1);
		aux = line;
		axis z = aux.toDouble();

		vectorList.append(Vec3D(x, y, z));

	}else if(line.contains("f ")){

		line.remove(0, 2);
		QString aux = line.left(line.indexOf(" "));
		if(aux.contains("/")){
			aux = line.left(line.indexOf("/"));
		}
		int f1 = aux.toInt() - 1;

		line.remove(0, line.indexOf(" ") +1);
		aux = line.left(line.indexOf(" "));
		if(aux.contains("/")){
			aux = line.left(line.indexOf("/"));
		}
		int f2 = aux.toInt() - 1;

		line.remove(0, line.indexOf(" ") +1);
		aux = line;
		if(aux.contains("/")){
			aux = line.left(line.indexOf("/"));
		}
		int f3 = aux.toInt() - 1;

		faceList.append(createFaceIndexes(f1, f2, f3, rgb));
	}
}

FaceIndexes Object3D::createFaceIndexes(int f1, int f2, int f3, Color color) {
	FaceIndexes face;
	face.f1 = f1;
	face.f2 = f2;
	face.f3 = f3;
	face.color = color;
	return face;
}

