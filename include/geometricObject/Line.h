/*
 * Line.h
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#ifndef LINE_H_
#define LINE_H_

#include "GeometricShape.h"

class Line : public GeometricShape{

public:
	Line(Vec3D vecStr, Vec3D vecEnd, Label name, Color rgb);
	Line(Vec3D vecStr, Vec3D vecEnd);
	Line():GeometricShape(){};

	virtual ~Line();

	const Vec3D& getVecEnd() const;
	void setVecEnd(const Vec3D& vecEnd);
	const Vec3D& getVecStr() const;
	void setVecStr(const Vec3D& vecStr);
	const Vec3D& getWvecEnd() const;
	void setWvecEnd(const Vec3D& wvecEnd);
	const Vec3D& getWvecStr() const;
	void setWvecStr(const Vec3D& wvecStr);


	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const;
	virtual VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList list) ;


private:
	Vec3D vecStr;
	Vec3D vecEnd;

	Vec3D WvecStr;
	Vec3D WvecEnd;
};

#endif /* LINE_H_ */
