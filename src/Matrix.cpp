#include "Matrix.h"

Matrix::Matrix(int n,int m) {
	this->n = n;
	this->m = m;
	for(int row = 0; row < this->n; row++)
	{
		QVector<axis> a;
		this->d.insert(row, a);
		for(int column = 0; column < this->m; column++)
			this->d[row].insert(column, 0);
	}
}

Matrix::Matrix(){
	this->n = 1;
	this->m = 1;
	for(int row = 0; row < this->n; row++)
	{
		QVector<axis> a;
		this->d.insert(row, a);
		for(int column = 0; column < this->m; column++)
			this->d[row].insert(column, 0);
	}
}

Matrix::~Matrix() {}

Matrix::Matrix(const Matrix& mtx) {
	this->n = mtx.getN();
	this->m = mtx.getM();
	this->d = QVector<QVector<axis> >(mtx.getD());
}

Matrix& Matrix::operator =(const Matrix& mtx) {
	this->n = mtx.getN();
	this->m = mtx.getM();
	this->d = QVector<QVector<axis> >(mtx.getD());
	return *this;
}

bool Matrix::insert(int n, int m, axis value)
{
	if(n < this->n && m < this->m)
	{
		this->d[n][m] = value;
		return true;
	}
	else
		return false;
}

int Matrix::getN() const{
	return this->n;
}

int Matrix::getM() const{
	return this->m;
}

QVector<QVector<axis> > Matrix::getD() const {
	return this->d;
}

axis& Matrix::operator ()(axis n, axis m) {
	if(n >= getN()){
		n = getN()-1;
	} else if(n < 0){
		n = 0;
	}

	if(m >= getM()){
		m = getM()-1;
	} else if( m < 0 ){
		m = 0;
	}

	return this->d[n][m];
}

Matrix Matrix::operator*(const Matrix& mtx) {
	if(getM() != mtx.getN())
		return Matrix(0,0);

	Matrix multiplied = Matrix(getN(), mtx.getM());
	for(int n = 0; n < getN(); n++)
		for(int m = 0; m < mtx.getM(); m++)
			multiplied(n,m) = multiplyRowColumn(n, mtx, m);
	return multiplied;
}

axis Matrix::multiplyRowColumn(int row, const Matrix& matrix, int column)
{
	axis value = 0;
	for(int currentPosition = 0; currentPosition < matrix.n; currentPosition++)
		value += this->operator ()(row, currentPosition)* matrix.d[currentPosition][column];

	return value;
}

void Matrix::print(std::string s)
{
	std::cout << s << std::endl;
	for(int row = 0; row < this->n; row++)
	{
		std::cout << "[";
		for(int column = 0; column < this->m; column++)
			std::cout << this->d[row].at(column) << " ";
		std::cout << "]" << std::endl;
	}
}
