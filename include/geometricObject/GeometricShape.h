/*
 * GeometricShape.h
 *
 *  Created on: Mar 21, 2013
 *      Author: perin
 */

#ifndef GEOMETRICSHAPE_H_
#define GEOMETRICSHAPE_H_

#include <QList>
#include <QString>
#include <math.h>
#include <QRgb>
#include "Vec3D.h"
#include "Matrix.h"
#include <iostream>
#include <QColor>

#define PI M_PI

typedef QList<Vec3D> VectorList;
typedef QString Label;
typedef QColor Color;

typedef double Angle;
typedef double Radian;

struct FaceIndexes {
	int f1;
	int f2;
	int f3;
	Color color;
};
typedef QList<FaceIndexes> FaceList;



class GeometricShape {
public:
	enum Type{
		POINT,
		LINE,
		WIREFRAME,
		WINDOW,
		BEZIER_CURVE_2D,
		B_SPLINE_FORWARD_DIFFERENCES,
		OBJECT3D
	};

	GeometricShape(){
		this->name = Label::null;
		this->rgb = QColor();
	};

	GeometricShape(Label name, Color rgb){
		this->name = name;
		this->rgb = rgb;
	};

	virtual ~GeometricShape(){};

	virtual VectorList getVectorList() const = 0;
	virtual void setVectorList( const VectorList vl) = 0;

	virtual VectorList getWVectorList() const = 0;
	virtual void setWVectorList( const VectorList vl) = 0;

	virtual FaceList getFaceList() const = 0;
	virtual void setFaceList(const FaceList) = 0;

	virtual GeometricShape::Type getType() const = 0;

	const Label& getName() const {
		return name;
	}

	void setName(const Label& name) {
		this->name = name;
	}

	virtual Vec3D center(){
		axis x,y,z = 0;
		VectorList vl = this->getWVectorList();
		if(vl.size() > 0)
		{
			for(int i = 0; i < vl.size(); i++)
			{
				x += vl.at(i).getX();
				y += vl.at(i).getY();
				z += vl.at(i).getZ();
			}

			x = x/(double)vl.size();
			y = y/(double)vl.size();
			z = z/(double)vl.size();
		}
		return Vec3D(x,y,z);
	};

	/*ROTACAO NAS COORDENADAS DO OBJETO RELATIVO AO MUNDO*/
	virtual void rotate(Angle angle, Vec3D vec, Vec3D axis){
		Vec3D center = vec;
		Vec3D negativeCenter = center.inverse();


		Matrix translateToOrigin(4,4);
		Matrix translateFromOrigin(4,4);
		Matrix rx(4,4);
		Matrix ry(4,4);
		Matrix rz(4,4);
		Matrix nrx(4,4);
		Matrix nrz(4,4);

		translateToOrigin(0,0) = 1;
		translateToOrigin(1,1) = 1;
		translateToOrigin(2,2) = 1;
		translateToOrigin(3,3) = 1;
		translateToOrigin(3,0) = negativeCenter.getX();
		translateToOrigin(3,1) = negativeCenter.getY();
		translateToOrigin(3,2) = negativeCenter.getZ();

		translateFromOrigin(0,0) = 1;
		translateFromOrigin(1,1) = 1;
		translateFromOrigin(2,2) = 1;
		translateFromOrigin(3,3) = 1;
		translateFromOrigin(3,0) = center.getX();
		translateFromOrigin(3,1) = center.getY();
		translateFromOrigin(3,2) = center.getZ();

		Vec3D axisProjection;
		bool useRz = true;
		if(axis.getX() == 0 && axis.getY() == 0 && axis.getZ() != 0){
			axisProjection = Vec3D(1,0,0);
			useRz = false;
		}else{

		axisProjection = Vec3D(axis.getX(), axis.getY());
		}

		Angle angleFromProjection = angleDiffFromAxys(axis, axisProjection);
		Angle angleFromProjToYAxis = angleDiffFromAxys(Vec3D(0,1,0), axisProjection);

		rx(0,0) = 1;
		rx(1,1) = cos(angleFromProjection);
		rx(1,2) = sin(angleFromProjection);
		rx(2,1) = -sin(angleFromProjection);
		rx(2,2) = cos(angleFromProjection);
		rx(3,3) = 1;


		ry(0,0) = cos(radian(angle));
		ry(0,2) = -sin(radian(angle));
		ry(1,1) = 1;
		ry(2,0) = sin(radian(angle));
		ry(2,2) = cos(radian(angle));
		ry(3,3) = 1;


		rz(0,0) = cos(angleFromProjToYAxis);
		rz(0,1) = sin(angleFromProjToYAxis);
		rz(1,0) = -sin(angleFromProjToYAxis);
		rz(1,1) = cos(angleFromProjToYAxis);
		rz(2,2) = 1;
		rz(3,3) = 1;


		nrx(0,0) = 1;
		nrx(1,1) = cos(-angleFromProjection);
		nrx(1,2) = sin(-angleFromProjection);
		nrx(2,1) = -sin(-angleFromProjection);
		nrx(2,2) = cos(-angleFromProjection);
		nrx(3,3) = 1;

		nrz(0,0) = cos(-angleFromProjToYAxis);
		nrz(0,1) = sin(-angleFromProjToYAxis);
		nrz(1,0) = -sin(-angleFromProjToYAxis);
		nrz(1,1) = cos(-angleFromProjToYAxis);
		nrz(2,2) = 1;
		nrz(3,3) = 1;

		Matrix multiplied;
		if(useRz){
			multiplied = translateToOrigin * rx * rz * ry * nrz * nrx * translateFromOrigin;
		}else{
			multiplied = translateToOrigin * rx * ry  * nrx * translateFromOrigin;
		}

		VectorList translated = VectorList();
		VectorList vl = this->getWVectorList();
		for(int i = 0; i < vl.size(); i++)
		{
			Matrix vector(1,4);
			vector(0,0) = vl.at(i).getX();
			vector(0,1) = vl.at(i).getY();
			vector(0,2) = vl.at(i).getZ();
			vector(0,3) = 1;

			vector = vector * multiplied;

			Vec3D vectorRotated(vector(0,0), vector(0,1), vector(0,2));
			translated.append(vectorRotated);
		}
		this->setWVectorList(translated);

	};

	virtual void updatePPCfromMatrix(Matrix m){
		VectorList translated = VectorList();
		VectorList vl = VectorList();
		vl.append(this->getWVectorList());
		for(int i = 0; i < vl.size(); i++)
		{
			Matrix vector(1,4);
					vector(0,0) = vl.at(i).getX();
					vector(0,1) = vl.at(i).getY();
					vector(0,2) = vl.at(i).getZ();
					vector(0,3) = 1;

					vector = vector * m;

					Vec3D vectorRotated(vector(0,0), vector(0,1), vector(0,2));
					translated.append(vectorRotated);
		}
		this->setVectorList(translated);
	}

	virtual void updatePPCToPerspective(){
		VectorList vl = VectorList();
		for(int i = 0; i < getVectorList().size(); i++){
			Vec3D vec = getVectorList().at(i);

			axis d = 100;
			axis z;
			z = vec.getZ();

			if(z != 0){
				vec.setX( vec.getX() / (z/d) );
				vec.setY( vec.getY() / (z/d) );
			}
			vl.append(vec);
		}
		setVectorList(vl);
	}


	virtual void translate(Vec3D vector){
		VectorList translated =  VectorList();
		VectorList vl = this->getWVectorList();
		for(int i = 0; i < vl.size(); i++)
			translated.insert(i, Vec3D(vl.at(i).getX() + vector.getX()
					,vl.at(i).getY() + vector.getY()
					,vl.at(i).getZ() + vector.getZ()));
		this->setWVectorList(translated);
	};


	virtual void scale(Vec3D factor)
	{
		Vec3D center = this->center();
		Vec3D negativeCenter = center.inverse();

		Matrix translateToOrigin(4,4);
		Matrix translateFromOrigin(4,4);
		Matrix scale(4,4);

		translateToOrigin(0,0) = 1;
		translateToOrigin(1,1) = 1;
		translateToOrigin(2,2) = 1;
		translateToOrigin(3,3) = 1;
		translateToOrigin(3,0) = negativeCenter.getX();
		translateToOrigin(3,1) = negativeCenter.getY();
		translateToOrigin(3,2) = negativeCenter.getZ();

		scale(0,0) = factor.getX();
		scale(1,1) = factor.getY();
		scale(2,2) = factor.getZ();
		scale(3,3) = 1;

		translateFromOrigin(0,0) = 1;
		translateFromOrigin(1,1) = 1;
		translateFromOrigin(2,2) = 1;
		translateFromOrigin(3,3) = 1;
		translateFromOrigin(3,0) = center.getX();
		translateFromOrigin(3,1) = center.getY();
		translateFromOrigin(3,2) = center.getZ();

		Matrix multiplied = translateToOrigin * scale * translateFromOrigin;

		VectorList translated = VectorList();
		VectorList vl = this->getWVectorList();
		for(int i = 0; i < vl.size(); i++)
		{
			Matrix vector(1,4);
			vector(0,0) = vl.at(i).getX();
			vector(0,1) = vl.at(i).getY();
			vector(0,2) = vl.at(i).getZ();
			vector(0,3) = 1;

			vector = vector * multiplied;

			Vec3D vectorRotated(vector(0,0), vector(0,1), vector(0,2));
			translated.insert(i, vectorRotated);
		}
		this->setWVectorList(translated);
	};

	static Radian radian(Angle angle){
		return angle * PI/180.0;
	}

	static Angle angle(Radian radian){
		return (radian * 180) / PI;
	}

	Color getRgb() const {
		return rgb;
	}

	void setRgb(Color rgb) {
		this->rgb = rgb;
	}

	static Radian angleDiffFromAxys(Vec3D axis1, Vec3D axis2){
		axis dotProd = axis1.dotProduct(axis2);
		axis norm1 = axis1.length();
		axis norm2 = axis2.length();

		if((norm1 * norm2) == 0){
			return 0;
		}else{
			return acos(dotProd / (norm1 * norm2)); //RETORNO EM RAD
		}

	}

protected:
	Label name;
	Color rgb;
};

#endif /* GEOMETRICSHAPE_H_ */
