/*
 * Point.h
 *
 *  Created on: Mar 25, 2013
 *      Author: lucasperin
 */

#ifndef POINT_H_
#define POINT_H_

#include "GeometricShape.h"

class Point : public GeometricShape {
public:

	Point(Vec3D vector, Label name, Color rgb);
	Point(Vec3D vector);
	virtual ~Point();

	const Vec3D& getVector() const;
	void setVector(const Vec3D& vector);
	const Vec3D& getWvector() const;
	void setWvector(const Vec3D& wvector);

	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const ;
	virtual VectorList getWVectorList() const ;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList) ;

private:
	Vec3D vector;
	Vec3D Wvector;
};

#endif /* POINT_H_ */
