/*
 * Object3D.h
 *
 *  Created on: May 13, 2013
 *      Author: lucasperin
 */

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include "GeometricShape.h"
#include "Line.h"
#include <QFile>

class Object3D: public GeometricShape {
public:
	Object3D(VectorList vectorList, FaceList faceList, bool brushed = false);
	Object3D(VectorList vectorList, FaceList faceList, Label name, Color rgb, bool brushed = false);
	Object3D(QString file, Label name, Color rgb, bool brushed = false);
	virtual ~Object3D();

	bool getBrushed();

	/* From GeometricShape */
	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const;
	virtual VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList list) ;


private:
	void buildObject(QString file);
	void iterateLine(QString line);
	FaceIndexes createFaceIndexes(int f1, int f2, int f3, Color color);
	VectorList vectorList;
	VectorList WvectorList;
	FaceList faceList;
	bool brushed;
};

#endif /* OBJECT3D_H_ */
