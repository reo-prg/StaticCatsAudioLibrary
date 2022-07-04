#include "../include/SCAL_Math.h"
#include <cmath>
#include "../include/StaticCatsAudioLibrary.h"

namespace scal
{
	Vector3 operator+(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x + val2.x, val1.y + val2.y, val1.z + val2.z };
	}

	Vector3 operator-(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x - val2.x, val1.y - val2.y, val1.z - val2.z };
	}

	Vector3 operator*(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x * val2.x, val1.y * val2.y, val1.z * val2.z };
	}

	Vector3 operator*(const Vector3& val1, float val2)
	{
		return { val1.x * val2, val1.y * val2, val1.z * val2 };
	}

	Vector3 operator/(const Vector3& val1, const Vector3& val2)
	{
		return { val1.x / val2.x, val1.y / val2.y, val1.z / val2.z };
	}

	Vector3 operator/(const Vector3& val1, float val2)
	{
		return { val1.x / val2, val1.y / val2, val1.z / val2 };
	}

	float Dot(const Vector3& val1, const Vector3& val2)
	{
		return val1.x * val2.x + val1.y * val2.y + val1.z * val2.z;
	}
	Vector3 Cross(const Vector3& val1, const Vector3& val2)
	{
		return { val1.y * val2.z - val1.z * val2.y,
			val1.z* val2.x - val1.x * val2.z, 
			val1.x* val2.y - val1.y * val2.x, };
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

	DirectX::XMFLOAT3 GetFloat3(const Vector3& val)
	{
		return { val.x, val.y, val.z };
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
		EulerOrder order = GetEulerOrder();

		Matrix m = IdentityMatrix();
		switch (order)
		{
		case EulerOrder::XYZ:
			m = RotationMatrixAxisX(x) * (RotationMatrixAxisY(y) * RotationMatrixAxisZ(z));
			break;
		case EulerOrder::XZY:
			m = RotationMatrixAxisX(x) * (RotationMatrixAxisY(z) * RotationMatrixAxisZ(y));
			break;
		case EulerOrder::YXZ:
			m = RotationMatrixAxisX(y) * (RotationMatrixAxisY(x) * RotationMatrixAxisZ(z));
			break;
		case EulerOrder::YZX:
			m = RotationMatrixAxisX(y) * (RotationMatrixAxisY(z) * RotationMatrixAxisZ(x));
			break;
		case EulerOrder::ZXY:
			m = RotationMatrixAxisX(z) * (RotationMatrixAxisY(x) * RotationMatrixAxisZ(y));
			break;
		case EulerOrder::ZYX:
			m = RotationMatrixAxisX(z) * (RotationMatrixAxisY(y) * RotationMatrixAxisZ(x));
			break;
		}
		return m;
	}

	Matrix RotationMatrix(const Vector3& vec)
	{
		return RotationMatrix(vec.x, vec.y, vec.z);
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
	Matrix TranslationMatrix(const Vector3& vec)
	{
		return TranslationMatrix(vec.x, vec.y, vec.z);
	}
	Matrix IdentityMatrix(void)
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Quaternion Quaternion::Inverse(void)
	{
		return Quaternion(-x, -y, -z, w);
	}

	Quaternion Quaternion::RotateAxis(const Vector3& axis, float rotateAngle)
	{
		return Quaternion(axis.x * sinf(rotateAngle / 2.0f), axis.y * sinf(rotateAngle / 2.0f),
			axis.z * sinf(rotateAngle / 2.0f), cosf(rotateAngle / 2.0f));
	}

	Quaternion Quaternion::EulerAngle(float angx, float angy, float angz)
	{
		Quaternion quat;
		quat.SetRotationEulerAngle(angx, angy, angz);
		return quat;
	}

	Vector3 Quaternion::RotateVector(const Vector3& vec)
	{
		Quaternion v = Quaternion(vec.x, vec.y, vec.z, 0.0f);
		Quaternion i = Inverse();
		Quaternion res = *this * v * i;
		return Vector3(res.x, res.y, res.z);
	}

	Matrix Quaternion::GetRotationMatrix(void)
	{
		Matrix ret = IdentityMatrix();

		ret.matrix_[0][0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
		ret.matrix_[0][1] = 2.0f * x * y + 2.0f * z * w;
		ret.matrix_[0][2] = 2.0f * x * z - 2.0f * y * w;
		ret.matrix_[1][0] = 2.0f * x * y - 2.0f * z * w;
		ret.matrix_[1][1] = 1.0f - 2.0f * x * x - 2.0f * z * z;
		ret.matrix_[1][2] = 2.0f * y * z + 2.0f * x * w;
		ret.matrix_[2][0] = 2.0f * x * z + 2.0f * y * w;
		ret.matrix_[2][1] = 2.0f * y * z - 2.0f * x * w;
		ret.matrix_[2][2] = 1.0f - 2.0f * x * x - 2.0f * y * y;

		return ret;
	}

	void Quaternion::SetRotationMatrix(const Matrix& mat)
	{
		float calc[4];
		calc[0] = mat.matrix_[0][0] - mat.matrix_[1][1] - mat.matrix_[2][2] + 1.0f;
		calc[1] = -mat.matrix_[0][0] + mat.matrix_[1][1] - mat.matrix_[2][2] + 1.0f;
		calc[2] = -mat.matrix_[0][0] - mat.matrix_[1][1] + mat.matrix_[2][2] + 1.0f;
		calc[3] = mat.matrix_[0][0] + mat.matrix_[1][1] + mat.matrix_[2][2] + 1.0f;

		int bigIdx = 0;
		for (int i = 1; i < 4; i++)
		{
			if (calc[i] > calc[bigIdx])
			{
				bigIdx = i;
			}
		}

		if (calc[bigIdx] < 0.0f) { return; }

		float val = sqrtf(calc[bigIdx]) * 0.5f;
		float m = 0.25f / val;
		calc[bigIdx] = m;

		switch (bigIdx)
		{
		case 0:
			calc[1] = (mat.matrix_[0][1] + mat.matrix_[1][0]) * m;
			calc[2] = (mat.matrix_[0][2] + mat.matrix_[2][0]) * m;
			calc[3] = (mat.matrix_[1][2] - mat.matrix_[2][1]) * m;
			break;
		case 1:
			calc[0] = (mat.matrix_[0][1] + mat.matrix_[1][0]) * m;
			calc[2] = (mat.matrix_[1][2] + mat.matrix_[2][1]) * m;
			calc[3] = (mat.matrix_[2][0] - mat.matrix_[0][2]) * m;
			break;
		case 2:
			calc[0] = (mat.matrix_[0][2] + mat.matrix_[2][0]) * m;
			calc[1] = (mat.matrix_[1][2] + mat.matrix_[2][1]) * m;
			calc[3] = (mat.matrix_[0][1] - mat.matrix_[1][0]) * m;
			break;
		case 3:
			calc[0] = (mat.matrix_[1][2] - mat.matrix_[2][1]) * m;
			calc[1] = (mat.matrix_[2][0] - mat.matrix_[0][2]) * m;
			calc[2] = (mat.matrix_[0][1] - mat.matrix_[1][0]) * m;
			break;
		}

		x = calc[0];
		y = calc[1];
		z = calc[2];
		w = calc[3];
	}

	void Quaternion::SetRotationEulerAngle(const Vector3& angle)
	{
		SetRotationEulerAngle(angle.x, angle.y, angle.z);
	}

	void Quaternion::SetRotationEulerAngle(float angx, float angy, float angz)
	{
		EulerOrder order = GetEulerOrder();

		float sx, sy, sz, cx, cy, cz;

		sx = sinf(angx * 0.5f);
		sy = sinf(angy * 0.5f);
		sz = sinf(angz * 0.5f);
		cx = cosf(angx * 0.5f);
		cy = cosf(angy * 0.5f);
		cz = cosf(angz * 0.5f);


		switch (order)
		{
		case EulerOrder::XYZ:
			x = cx * sy * sz + sx * cy * cz;
			y = -sx * cy * sz + cx * sy * cz;
			z = cx * cy * sz + sx * sy * cz;
			w = -sx * sy * sz + cx * cy * cz;
			break;
		case EulerOrder::XZY:
			x = -cx * sy * sz + sx * cy * cz;
			y = cx * sy * cz - sx * cy * sz;
			z = sx * sy * cz + cx * cy * sz;
			w = sx * sy * sz + cx * cy * cz;
			break;
		case EulerOrder::YXZ:
			x = cx * sy * sz + sx * cy * cz;
			y = -sx * cy * sz + cx * sy * cz;
			z = cx * cy * sz + sx * sy * cz;
			w = sx * sy * sz + cx * cy * cz;
			break;
		case EulerOrder::YZX:
			x = sx * cy * cz + cx * sy * sz;
			y = sx * cy * sz + cx * sy * cz;
			z = -sx * sy * cz + cx * cy * sz;
			w = -sx * sy * sz + cx * cy * cz;
			break;
		case EulerOrder::ZXY:
			x = -cx * sy * sz + sx * cy * cz;
			y = cx * sy * cz + sx * cy * sz;
			z = sx * sy * cz + cx * cy * sz;
			w = -sx * sy * sz + cx * cy * cz;
			break;
		case EulerOrder::ZYX:
			x = sx * cy * cz - cx * sy * sz;
			y = sx * cy * sz + cx * sy * cz;
			z = -sx * sy * cz + cx * cy * sz;
			w = sx * sy * sz + cx * cy * cz;
			break;
		}
	}

	void Quaternion::SetRotationAxis(const Vector3& axis, float rotateAngle)
	{
		x = axis.x * sinf(rotateAngle / 2.0f);
		y = axis.y * sinf(rotateAngle / 2.0f);
		z = axis.z * sinf(rotateAngle / 2.0f);
		w = cosf(rotateAngle / 2.0f);
	}
}
