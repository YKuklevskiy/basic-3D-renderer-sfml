#pragma once
#include "vector"
class Matrix
{
public:
	Matrix();
	Matrix(int rows, int columns);
	Matrix(std::vector<float> v);
	Matrix(std::vector<std::vector<float>> v);
	
	Matrix transpose();
	void printMatrix();
	void fill(float f);
	std::vector<float>& operator[](int i);
	Matrix operator +(const Matrix& m);
	Matrix operator -(const Matrix& m);
	Matrix operator *(float s);
	Matrix operator /(float s);
	Matrix operator *(const Matrix& m);

	int getRowCount();
	int getColumnCount();

private:
	// first: rows, second: columns
	std::pair<int, int> size;
	std::vector<std::vector<float>> mat;
};

