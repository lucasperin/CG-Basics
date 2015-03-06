#ifndef BEZIERCURVE2D_H_
#define BEZIERCURVE2D_H_

#include "GeometricShape.h"
#include "Vec3D.h"

class BezierCurve2D: public GeometricShape {
public:
	BezierCurve2D(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4);
	BezierCurve2D(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Label name, Color rgb);
	BezierCurve2D():GeometricShape(){};
	virtual ~BezierCurve2D(){};

	VectorList getVectorList() const;
	void setVectorList( const VectorList vl);
	GeometricShape::Type getType() const;
	VectorList getWVectorList() const;
	void setWVectorList( const VectorList vl);
	virtual FaceList getFaceList() const;
	virtual void setFaceList(const FaceList) ;

	//calcula a reta a partir de um lista de pontos p1,p2,p3,p4
	VectorList calculateCurve(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4);

protected:
	Vec3D p1,p2,p3,p4;
	Vec3D Wp1,Wp2,Wp3,Wp4;

	VectorList vectorList;
	VectorList WvectorList;
};

#endif /* BEZIERCURVE2D_H_ */
