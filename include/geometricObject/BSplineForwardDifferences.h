/*
 * BSplineForwardDifferences.h
 *
 *  Created on: 02/05/2013
 *      Author: gabriel
 */

#ifndef BSPLINEFORWARDDIFFERENCES_H_
#define BSPLINEFORWARDDIFFERENCES_H_

#include "GeometricShape.h"

class BSpline_ForwardDifferences: public GeometricShape {
public:
	BSpline_ForwardDifferences(VectorList vl, Label name, Color rgb);
	virtual ~BSpline_ForwardDifferences();

	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const;
	virtual VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList) ;


protected:
	void calculateCurve(VectorList vl);
	Matrix initBS();
	Matrix initE(double a);
	void calcFwdDiff(int n, Matrix cx, Matrix cy);
	VectorList vectorList;
	VectorList WvectorList;

};

#endif /* BSPLINEFORWARDDIFFERENCES_H_ */
