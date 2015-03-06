/*
 * DisplayFiles.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: perin
 */

#include "DisplayFiles.h"

DisplayFiles::DisplayFiles() {
	this->fileList = new FileList();
}

DisplayFiles::~DisplayFiles() {
	delete this->fileList;
}

void DisplayFiles::addGeometricShape(GeometricShape* gs)
{
	this->fileList->insert(gs->getName(), gs);
}

GeometricShape* DisplayFiles::getGeometricShape(Label name)
{
	return this->fileList->value(name);
}

void DisplayFiles::addPoint(Label name, Vec3D p1, Color rgb)
{
	Point* point = new Point(p1, name, rgb);

	this->addGeometricShape(point);
}

void DisplayFiles::addLine(Label name, Vec3D p1, Vec3D p2, Color rgb)
{

	Line* line = new Line(p1, p2, name, rgb);

	this->addGeometricShape(line);
}


void DisplayFiles::addPolygon(Label name, VectorList vl, Color rgb, int opacity, bool brushed)
{
	rgb.setAlpha(opacity);
	WireFrame* wf = new WireFrame(vl, name, rgb, brushed);
	this->addGeometricShape(wf);

}
void DisplayFiles::addBezierCurve(Label name, Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Color rgb)
{
	BezierCurve2D* bc = new BezierCurve2D(p1, p2, p3, p4, name, rgb);
	this->addGeometricShape(bc);
}

void DisplayFiles::addBSpline(Label name, VectorList vl, Color rgb)
{
	BSpline_ForwardDifferences* bspline = new BSpline_ForwardDifferences(vl, name, rgb);
	this->addGeometricShape(bspline);

}

void DisplayFiles::addObject3D(QString file, Label name, Color rgb)
{
	Object3D* object = new Object3D(file, name, rgb);
	addGeometricShape(object);
}

bool DisplayFiles::contains(Label name)
{
	return this->fileList->contains(name);
}

FileList* DisplayFiles::getFileList()
{
	return this->fileList;
}

void DisplayFiles::removeObject(Label name)
{
	if(fileList->contains(name)){
		this->fileList->remove(name);
	}
}

void DisplayFiles::rotate(Label name, Angle angle, Vec3D vecStr, Vec3D vecEnd)
{
	this->getGeometricShape(name)->rotate(angle, vecStr, vecEnd);
}

void DisplayFiles::scale(Label name, Vec3D factor)
{
	this->getGeometricShape(name)->scale(factor);
}

GeometricShape::Type DisplayFiles::getGeometricShapeType(Label name) {
	return this->getGeometricShape(name)->getType();
}

void DisplayFiles::translate(Label name, Vec3D vec) {
	this->getGeometricShape(name)->translate(vec);
}


void DisplayFiles::print(Vec3D vec)
{
	std::cout << vec.getX() << ":" << vec.getY() << ":" << vec.getZ() << std::endl;
}

void DisplayFiles::viewProjection(Window* wd)
{

	Vec3D dotProd = wd->vUp.crossProduct(wd->vPN);

	Matrix m(4,4);
	m(0,0) = dotProd.getX();
	m(0,1) = dotProd.getY();
	m(0,2) = dotProd.getZ();

	m(1,0) = wd->vUp.getX();
	m(1,1) = wd->vUp.getY();
	m(1,2) = wd->vUp.getZ();

	m(2,0) = wd->vPN.getX();
	m(2,1) = wd->vPN.getY();
	m(2,2) = wd->vPN.getZ();
	m(3,3) = 1;

	Matrix t(4,4);
	t(0,0) = 1;
	t(1,1) = 1;
	t(2,2) = 1;
	t(3,0) = -wd->center().getX();
	t(3,1) = -wd->center().getY();
	t(3,2) = -wd->center().getZ();
	t(3,3) = 1;

//	Matrix nt(4,4);
//	nt(0,0) = 1;
//	nt(1,1) = 1;
//	nt(2,2) = 1;
//	nt(3,0) = wd->center().getX();
//	nt(3,1) = wd->center().getY();
//	nt(3,2) = wd->center().getZ();
//	nt(3,3) = 1;


	LabelList keys = this->fileList->keys();

	for(int i  = 0; i < keys.size(); i++){
		if(this->fileList->value(keys.at(i))->getType() != GeometricShape::WINDOW){
			GeometricShape* gm = this->getGeometricShape(keys.at(i));
			gm->updatePPCfromMatrix(t *m );
			gm->updatePPCToPerspective();
		}
	}

}

