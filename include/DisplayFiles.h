/*
 * DisplayFiles.h
 *
 *  Created on: Mar 25, 2013
 *      Author: perin
 */

#ifndef DISPLAYFILES_H_
#define DISPLAYFILES_H_

#include <QMap>
#include <QString>
#include <QList>
#include <QColor>
#include "GeometricShape.h"
#include "Window.h"
#include "geometricObject/Line.h"
#include "geometricObject/Point.h"
#include "geometricObject/WireFrame.h"
#include "geometricObject/BezierCurve2D.h"
#include "geometricObject/BSplineForwardDifferences.h"
#include "geometricObject/Object3D.h"

typedef QMap<QString, GeometricShape*> FileList;
typedef QList<Label> LabelList;

class DisplayFiles {
public:
	DisplayFiles();
	virtual ~DisplayFiles();
	void addGeometricShape(GeometricShape* gs);
	GeometricShape* getGeometricShape(Label name);
	GeometricShape::Type getGeometricShapeType(Label name);
	void addPoint(Label name, Vec3D p1, Color rgb);
	void addLine(Label name, Vec3D p1, Vec3D p2, Color rgb);
	void addPolygon(Label name, VectorList vl, Color rgb, int opacity, bool brushed);
	void addBezierCurve(Label name, Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Color rgb);
	void addBSpline(Label name, VectorList vl, Color rgb);
	void addObject3D(QString file, Label name, Color rgb);
	bool contains(Label name);
	FileList* getFileList();
	void removeObject(Label name);

	void rotate(Label name, Angle angle, Vec3D vecStr, Vec3D vecEnd);
	void scale(Label name, Vec3D factor);
	void translate(Label name, Vec3D vec);

	void viewProjection(Window* wd);

private:
	FileList* fileList;

	void print(Vec3D vec);
};

#endif /* DISPLAYFILES_H_ */
