#pragma once
#include "vector"
class Matrix
{
public:
	//
	//	Constructors
	//
	Matrix();
	Matrix(int rows, int columns);
	Matrix(std::vector<float> v);
	Matrix(std::vector<std::vector<float>> v);
	
	// Transposes this* matrix and returns a copy of result
	Matrix transpose(); 

	// Prints the matrix in the console
	void printMatrix(); // TODO beautify
	
	// Fills all the elements of the matrix with value of f
	void fill(float f);

	// Returns the identity matrix of requested size
	Matrix getIdentityMatrix(int size);
	
	// Returns reference to the row of the matrix by index i
	std::vector<float>& operator[](int i);
	
	Matrix operator +(const Matrix& m);
	Matrix operator -(const Matrix& m);
	Matrix operator *(float s);
	Matrix operator /(float s);
	Matrix operator *(const Matrix& m);

	// Get amount of rows in matrix
	int getRowCount();
	// Get amount of columns in matrix
	int getColumnCount();

private:

	// first: rows, second: columns
	std::pair<int, int> size;
	std::vector<std::vector<float>> mat;
};

