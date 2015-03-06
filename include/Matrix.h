#ifndef MATRIX_H_
#define MATRIX_H_

#include <QVector>
#include "geometricObject/Vec3D.h"
#include <string>
#include <iostream>

class Matrix {
public:
	Matrix(int n,int m);
	Matrix();
	virtual ~Matrix();
	Matrix(const Matrix &mtx);

	bool insert(int n, int m, axis value);

	axis multiplyRowColumn(int row, const Matrix& matrix, int column);

	int getN() const;
	int getM() const;
	QVector<QVector< axis> > getD() const;

	/* OPERATORS */
	Matrix& operator=(const Matrix &mtx);
	axis& operator()(axis n, axis m);
	Matrix operator*(const Matrix &mtx);

	void print(std::string s);

protected:
	QVector<QVector< axis> > d;
	int n,m;
};



#endif /* MATRIX_H_ */
