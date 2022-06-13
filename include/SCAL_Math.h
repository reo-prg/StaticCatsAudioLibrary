#pragma once

namespace scal
{
	class Vector3
	{
	public:
		float x_, y_, z_;

		Vector3(float x, float y, float z)
			: x_(x), y_(y), z_(z) {};

		Vector3() = default;
		~Vector3() = default;

		Vector3(const Vector3&) = default;
		Vector3& operator=(const Vector3&) = default;

		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;
	};

	Vector3 operator+(const Vector3& val1, const Vector3& val2);
	Vector3 operator-(const Vector3& val1, const Vector3& val2);
	Vector3 operator*(const Vector3& val1, const Vector3& val2);
	Vector3 operator*(const Vector3& val1, float val2);
	Vector3 operator/(const Vector3& val1, const Vector3& val2);
	Vector3 operator/(const Vector3& val1, float val2);

	float Dot(const Vector3& val1, const Vector3& val2);
	Vector3 Cross(const Vector3& val1, const Vector3& val2);

	Vector3 Normalize(const Vector3& val);

	float Length(const Vector3& val);
	float SquareLength(const Vector3& val);


	class Matrix
	{
	public:
		float matrix_[4][4];

		Matrix(float v00, float v01, float v02, float v03,
			float v10, float v11, float v12, float v13,
			float v20, float v21, float v22, float v23,
			float v30, float v31, float v32, float v33);

		~Matrix() = default;

		Matrix operator*(const Matrix& m);
	};

	Matrix RotationMatrix(float x, float y, float z);

	Matrix RotationMatrixAxisX(float x);
	Matrix RotationMatrixAxisY(float y);
	Matrix RotationMatrixAxisZ(float z);


	Matrix TranslationMatrix(float x, float y);

	Matrix IdentityMatrix(void);


	class Quaternion
	{
	public:
		float x_, y_, z_, w_;
	};
}
