/*
 * WireFrame.h
 *
 *  Created on: Mar 25, 2013
 *      Author: perin
 */

#ifndef WIREFRAME_H_
#define WIREFRAME_H_

#include "GeometricShape.h"
#include <iostream>

class WireFrame: public GeometricShape {
public:
	WireFrame(VectorList list, Label name, Color rgb, bool brushed = false);
	WireFrame(VectorList list, bool brushed = false);
	virtual ~WireFrame();

	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const;
	virtual VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList) ;

	void setBrushed(bool brushed);
	bool getBrushed();

private:
	VectorList vectorList;
	VectorList WvectorList;
	bool brushed;
};

#endif /* WIREFRAME_H_ */
