#pragma once
#include <DirectXMath.h>

namespace scal
{
	class Vector3
	{
	public:
		float x, y, z;

		Vector3(float valx, float valy, float valz)
			: x(valx), y(valy), z(valz) {};

		Vector3(const DirectX::XMFLOAT3& val) : x(val.x), y(val.y), z(val.z) {};

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

	float Distance(const Vector3& val1, const Vector3& val2);
	float SquareDistance(const Vector3& val1, const Vector3& val2);

	DirectX::XMFLOAT3 GetFloat3(const Vector3& val);

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
	Matrix RotationMatrix(const Vector3& vec);

	Matrix RotationMatrixAxisX(float x);
	Matrix RotationMatrixAxisY(float y);
	Matrix RotationMatrixAxisZ(float z);


	Matrix TranslationMatrix(float x, float y, float z);
	Matrix TranslationMatrix(const Vector3& vec);

	Matrix IdentityMatrix(void);


	class Quaternion
	{
	public:
		float x, y, z, w;

		Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};

		Quaternion(float valx, float valy, float valz, float valw) : x(valx), y(valy), z(valz), w(valw) {};

		Quaternion operator*(const Quaternion& quat)
		{
			return Quaternion(x * quat.w - y * quat.z + z * quat.y + w * quat.x,
				x * quat.z + y * quat.w - z * quat.x + w * quat.y,
				-x * quat.y + y * quat.x + z * quat.w + w * quat.z,
				-x * quat.x - y * quat.y - z * quat.z - w * quat.w);
		};

		Quaternion Inverse(void);

		Vector3 RotateVector(const Vector3& vec);

		Matrix GetRotationMatrix(void);

		void SetRotationMatrix(const Matrix& mat);

		void SetRotationEulerAngle(const Vector3& angle);

		void SetRotationEulerAngle(float angx, float angy, float angz);

		void SetRotationAxis(const Vector3& axis, float rotateAngle);

		static Quaternion RotateAxis(const Vector3& axis, float rotateAngle);

		static Quaternion EulerAngle(float angx, float angy, float angz);

	private:
	};

	namespace xm_operator
	{
		inline DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& val1, const DirectX::XMFLOAT3& val2)
		{
			return { val1.x + val2.x, val1.y + val2.y, val1.z + val2.z };
		}

		inline DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val1, const DirectX::XMFLOAT3& val2)
		{
			return { val1.x - val2.x, val1.y - val2.y, val1.z - val2.z };
		}	
	}
}
