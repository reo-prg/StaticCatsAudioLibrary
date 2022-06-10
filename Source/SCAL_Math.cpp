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
}
