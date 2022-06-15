#include "../include/SCAL_Math.h"
#include <cmath>

namespace scal
{
	Vector3 operator+(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x_ + val2.x_, val1.y_ + val2.y_, val1.z_ + val2.z_ };
	}

	Vector3 operator-(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x_ - val2.x_, val1.y_ - val2.y_, val1.z_ - val2.z_ };
	}

	Vector3 operator*(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x_ * val2.x_, val1.y_ * val2.y_, val1.z_ * val2.z_ };
	}

	Vector3 operator*(const Vector3& val1, float val2)
	{
		return { val1.x_ * val2, val1.y_ * val2, val1.z_ * val2 };
	}

	Vector3 operator/(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x_ / val2.x_, val1.y_ / val2.y_, val1.z_ / val2.z_ };
	}

	Vector3 operator/(const Vector3& val1, float val2)
	{
		return { val1.x_ / val2, val1.y_ / val2, val1.z_ / val2 };
	}

	float Dot(const Vector3& val1, const Vector3& val2)
	{
		return val1.x_ * val2.x_ + val1.y_ * val2.y_ + val1.z_ * val2.z_;
	}
	Vector3 Cross(const Vector3& val1, const Vector3& val2)
	{
		return { val1.y_ * val2.z_ - val1.z_ * val2.y_,
			val1.z_* val2.x_ - val1.x_ * val2.z_, 
			val1.x_* val2.y_ - val1.y_ * val2.x_, };
	}
	Vector3 Normalize(const Vector3& val)
	{
		return val / Length(val);
	}
	float Length(const Vector3& val)
	{
		return sqrtf(Dot(val, val));
	}
	float SquareLength(const Vector3& val)
	{
		return Dot(val, val);
	}

	float Distance(const Vector3& val1, const Vector3& val2)
	{
		return Length(val2 - val1);
	}

	float SquareDistance(const Vector3& val1, const Vector3& val2)
	{
		return SquareLength(val2 - val1);
	}
	
	Matrix::Matrix(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23,
		float v30, float v31, float v32, float v33)
	{
		matrix_[0][0] = v00;
		matrix_[0][1] = v01;
		matrix_[0][2] = v02;
		matrix_[0][3] = v03;
		matrix_[1][0] = v10;
		matrix_[1][1] = v11;
		matrix_[1][2] = v12;
		matrix_[1][3] = v13;
		matrix_[2][0] = v20;
		matrix_[2][1] = v21;
		matrix_[2][2] = v22;
		matrix_[2][3] = v23;
		matrix_[3][0] = v30;
		matrix_[3][1] = v31;
		matrix_[3][2] = v32;
		matrix_[3][3] = v33;
	}

	Matrix Matrix::operator*(const Matrix& m)
	{
		return Matrix(matrix_[0][0] * m.matrix_[0][0] + matrix_[0][1] * m.matrix_[1][0] + matrix_[0][2] * m.matrix_[2][0] + matrix_[0][3] * m.matrix_[3][0],
			matrix_[0][0] * m.matrix_[0][1] + matrix_[0][1] * m.matrix_[1][1] + matrix_[0][2] * m.matrix_[2][1] + matrix_[0][3] * m.matrix_[3][1],
			matrix_[0][0] * m.matrix_[0][2] + matrix_[0][1] * m.matrix_[1][2] + matrix_[0][2] * m.matrix_[2][2] + matrix_[0][3] * m.matrix_[3][2],
			matrix_[0][0] * m.matrix_[0][3] + matrix_[0][1] * m.matrix_[1][3] + matrix_[0][2] * m.matrix_[2][3] + matrix_[0][3] * m.matrix_[3][3],
			matrix_[1][0] * m.matrix_[0][0] + matrix_[1][1] * m.matrix_[1][0] + matrix_[1][2] * m.matrix_[2][0] + matrix_[1][3] * m.matrix_[3][0],
			matrix_[1][0] * m.matrix_[0][1] + matrix_[1][1] * m.matrix_[1][1] + matrix_[1][2] * m.matrix_[2][1] + matrix_[1][3] * m.matrix_[3][1],
			matrix_[1][0] * m.matrix_[0][2] + matrix_[1][1] * m.matrix_[1][2] + matrix_[1][2] * m.matrix_[2][2] + matrix_[1][3] * m.matrix_[3][2],
			matrix_[1][0] * m.matrix_[0][3] + matrix_[1][1] * m.matrix_[1][3] + matrix_[1][2] * m.matrix_[2][3] + matrix_[1][3] * m.matrix_[3][3],
			matrix_[2][0] * m.matrix_[0][0] + matrix_[2][1] * m.matrix_[1][0] + matrix_[2][2] * m.matrix_[2][0] + matrix_[2][3] * m.matrix_[3][0],
			matrix_[2][0] * m.matrix_[0][1] + matrix_[2][1] * m.matrix_[1][1] + matrix_[2][2] * m.matrix_[2][1] + matrix_[2][3] * m.matrix_[3][1],
			matrix_[2][0] * m.matrix_[0][2] + matrix_[2][1] * m.matrix_[1][2] + matrix_[2][2] * m.matrix_[2][2] + matrix_[2][3] * m.matrix_[3][2],
			matrix_[2][0] * m.matrix_[0][3] + matrix_[2][1] * m.matrix_[1][3] + matrix_[2][2] * m.matrix_[2][3] + matrix_[2][3] * m.matrix_[3][3],
			matrix_[3][0] * m.matrix_[0][0] + matrix_[3][1] * m.matrix_[1][0] + matrix_[3][2] * m.matrix_[2][0] + matrix_[3][3] * m.matrix_[3][0],
			matrix_[3][0] * m.matrix_[0][1] + matrix_[3][1] * m.matrix_[1][1] + matrix_[3][2] * m.matrix_[2][1] + matrix_[3][3] * m.matrix_[3][1],
			matrix_[3][0] * m.matrix_[0][2] + matrix_[3][1] * m.matrix_[1][2] + matrix_[3][2] * m.matrix_[2][2] + matrix_[3][3] * m.matrix_[3][2],
			matrix_[3][0] * m.matrix_[0][3] + matrix_[3][1] * m.matrix_[1][3] + matrix_[3][2] * m.matrix_[2][3] + matrix_[3][3] * m.matrix_[3][3]);
	}

	Matrix RotationMatrix(float x, float y, float z)
	{
		return RotationMatrixAxisZ(z) * (RotationMatrixAxisY(y) * RotationMatrixAxisX(x));
	}

	Matrix RotationMatrixAxisX(float x)
	{
		Matrix m = IdentityMatrix();

		m.matrix_[1][1] = cosf(x);
		m.matrix_[1][2] = -sinf(x);
		m.matrix_[2][1] = sinf(x);
		m.matrix_[2][2] = cosf(x);

		return m;
	}

	Matrix RotationMatrixAxisY(float y)
	{
		Matrix m = IdentityMatrix();

		m.matrix_[0][0] = cosf(y);
		m.matrix_[0][2] = sinf(y);
		m.matrix_[2][0] = -sinf(y);
		m.matrix_[2][2] = cosf(y);

		return m;
	}

	Matrix RotationMatrixAxisZ(float z)
	{
		Matrix m = IdentityMatrix();

		m.matrix_[0][0] = cosf(z);
		m.matrix_[0][1] = -sinf(z);
		m.matrix_[1][0] = sinf(z);
		m.matrix_[1][1] = cosf(z);

		return m;
	}

	Matrix TranslationMatrix(float x, float y, float z)
	{
		return Matrix(1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	Matrix IdentityMatrix(void)
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
}
