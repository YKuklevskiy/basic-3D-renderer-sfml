#include "matrix.h"
#include <iostream>

Matrix::Matrix()
{
	size.first = 0;
	size.second = 0;
}

Matrix::Matrix(int rows, int columns)
	: size{ rows, columns }
{
	for (int i = 0; i < rows; i++)
		mat.push_back(std::vector<float>(columns, 0));
}

Matrix::Matrix(std::vector<float> v) : Matrix::Matrix()
{
	if (!v.size())
		return;
	size.first = 1;
	size.second = v.size();
	mat.push_back(v);
}

Matrix::Matrix(std::vector<std::vector<float>> v) : Matrix::Matrix()
{
	if (!v.size())
		return;
	int temp = v[0].size();

	for (int i = 1; i < v.size(); i++)
		if (v[i].size() != temp)
			throw std::exception("Trying to create Matrix with dynamic row length");
	
	size.first = v.size();
	size.second = v[0].size();
	mat = v;
}

Matrix Matrix::transpose()
{
	Matrix newM(size.second, size.first);
	for (int i = 0; i < size.second; i++)
	{
		for (int j = 0; j < size.first; j++)
		{
			newM[i][j] = mat[j][i];
		}
	}
	*this = Matrix(newM);
	return newM;
}

void Matrix::printMatrix()
{
	//std::cout << "Printing matrix!" << std::endl;
	for (int i = 0; i < size.first; i++)
	{
		for (int j = 0; j < size.second; j++)
		{
			std::cout << mat[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void Matrix::fill(float f)
{
	for (int i = 0; i < size.first; i++)
		for (int j = 0; j < size.second; j++)
			mat[i][j] = f;
}

std::vector<float>& Matrix::operator[](int i)
{
	if (i < 0 || i >= size.first)
		throw std::exception("Invalid matrix row index");
	return mat[i];
}

Matrix Matrix::operator+(const Matrix& m)
{
	if(size.first != m.size.first || size.second != m.size.second)
		throw std::exception("Invalid matrix operation");
	
	for (int i = 0; i < size.first; i++)
		for (int j = 0; j < size.second; j++)
			mat[i][j] += m.mat[i][j];

	return *this;
}

Matrix Matrix::operator-(const Matrix& m)
{
	if (size.first != m.size.first || size.second != m.size.second)
		throw std::exception("Invalid matrix operation");

	for (int i = 0; i < size.first; i++)
		for (int j = 0; j < size.second; j++)
			mat[i][j] -= m.mat[i][j];

	return *this;
}

Matrix Matrix::operator*(float s)
{
	for (int i = 0; i < size.first; i++)
		for (int j = 0; j < size.second; j++)
			mat[i][j] *= s;

	return *this;
}

Matrix Matrix::operator/(float s)
{
	if (!s)
		throw std::exception("Division by zero");
	s = 1 / s; // Multiplying is faster than dividing
	for (int i = 0; i < size.first; i++)
		for (int j = 0; j < size.second; j++)
			mat[i][j] *= s;

	return *this;
}

Matrix Matrix::operator*(const Matrix& m)
{
	if (size.second != m.size.first)
		throw std::exception("Invalid matrix operation");

	Matrix newM(size.first, m.size.second);

	for (int i = 0; i < size.first; i++)
	{
		for (int j = 0; j < m.size.second; j++)
		{
			float sum = 0.0f;
			for (int k = 0; k < m.size.first; k++)
			{
				sum += mat[i][k] * m.mat[k][j];
			}
			newM[i][j] = sum;
		}
	}
	*this = Matrix(newM);
	return newM;
}

int Matrix::getRowCount()
{
	return size.first;
}

int Matrix::getColumnCount()
{
	return size.second;
}
