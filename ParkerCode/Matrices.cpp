#include "Matrices.h"

namespace Matrices
{
	//Matrix Constructor
	Matrix::Matrix(int _rows, int _cols)
	{
		a = vector<vector<double>>(_rows, vector<double>(_cols, 0));
		rows = _rows;
		cols = _cols;
	}

	///Add each corresponding element.
	///usage: c = a + b;
	///Exception: runtime_error
	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
			throw runtime_error("Error: dimensions must agree");
		}

		Matrix c(a.getRows(), a.getCols());

		for (int i = 0; i < a.getRows(); i++)
		{
			for (int j = 0; j < a.getCols(); j++)
			{
				c(i, j) = a(i, j) + b(i, j);
			}
		}

		return c;
	}

	///Matrix multiply.
	///usage: c = a * b;
	Matrix operator*(const Matrix& a, const Matrix& b)
	{
		if (a.getCols() != b.getRows())
		{
			throw runtime_error("Error: dimensions must agree");
		}

		Matrix c(a.getRows(), b.getCols());

		for (int arow = 0; arow < a.getRows(); arow++)
		{
			for (int bcol = 0; bcol < b.getCols(); bcol++)
			{
				//calculate c(arow,bcol)
				//acol = brow = x
				for (int x = 0; x < a.getCols(); x++)
				{
					c(arow, bcol) += a(arow, x) * b(x, bcol);
				}
			}
		}

		return c;
	}

	///Matrix comparison.
	///usage: a == b
	bool operator==(const Matrix& a, const Matrix& b)
	{
		//check if rows and cols are equal
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
		{
			return false;
		}

		//check if values are equal
		for (int i = 0; i < a.getRows(); i++)
		{
			for (int j = 0; j < a.getCols(); j++)
			{
				if (abs(a(i, j) - b(i, j)) > 0.001)
				{
					return false;
				}
			}
		}
		return true;
	}

	///Matrix !comparison.
	///usage: a != b
	bool operator!=(const Matrix& a, const Matrix& b)
	{
		return !(a == b);
	}

	///Output matrix.
	///Separate columns by ' ' and rows by '\n'
	ostream& operator<<(ostream& os, const Matrix& a)
	{
		for (int i = 0; i < a.getRows(); i++)
		{
			for (int j = 0; j < a.getCols(); j++)
			{
				cout << setw(10) << a(i, j);
				if (j + 1 < a.getCols()) { cout << ' '; }
			}
			cout << '\n';
		}

		return os;
	}

	///Call the parent constructor to create a 2x2 matrix
	///Then assign each element as follows:
	/*
	cos(theta) -sin(theta)
	sin(theta) cos(theta)
	*/
	///theta represents the angle of rotation in radians, counter-clockwise
	RotationMatrix::RotationMatrix(double theta) : Matrix(2,2)
	{
		a[0][0] = cos(theta);
		a[0][1] = -sin(theta);
		a[1][0] = sin(theta);
		a[1][1] = cos(theta);
	}

	///Call the parent constructor to create a 2x2 matrix
	///Then assign each element as follows:
	/*
	scale 0
	0 scale
	*/
	///scale represents the size multiplier
	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2,2)
	{
		a[0][0] = a[1][1] = scale;
		a[0][1] = a[1][0] = 0;
	}

	///Call the parent constructor to create a 2xn matrix
	///Then assign each element as follows:
	/*
	xShift xShift xShift ...
	yShift yShift yShift ...
	*/
	///paramaters are xShift, yShift, and nCols
	///nCols represents the number of columns in the matrix
	///where each column contains one (x,y) coordinate pair
	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2,nCols)
	{
		for (int col = 0; col < nCols; col++)
		{
			a[0][col] = xShift;
			a[1][col] = yShift;
		}
	}
}