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
}
