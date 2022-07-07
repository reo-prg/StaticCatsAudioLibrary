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

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="val1">1つ目の値</param>
	/// <param name="val2">2つ目の値</param>
	/// <returns>val1 ･ val2</returns>
	float Dot(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="val1">1つ目の値</param>
	/// <param name="val2">2つ目の値</param>
	/// <returns>val1 × val2</returns>
	Vector3 Cross(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="val">ベクトル</param>
	/// <returns>正規化ベクトル</returns>
	Vector3 Normalize(const Vector3& val);

	/// <summary>
	/// ベクトルの長さ
	/// </summary>
	/// <param name="val">ベクトル</param>
	/// <returns>長さ</returns>
	float Length(const Vector3& val);

	/// <summary>
	/// ベクトルの長さの2乗
	/// </summary>
	/// <param name="val">ベクトル</param>
	/// <returns>長さの2乗</returns>
	float SquareLength(const Vector3& val);

	/// <summary>
	/// 2点間の距離
	/// </summary>
	/// <param name="val1">1つ目の点</param>
	/// <param name="val2">2つ目の点</param>
	/// <returns>距離</returns>
	float Distance(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// 2点間の距離の2乗
	/// </summary>
	/// <param name="val1">1つ目の点</param>
	/// <param name="val2">2つ目の点</param>
	/// <returns>距離の2乗</returns>
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

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <param name="x">x回転</param>
	/// <param name="y">y回転</param>
	/// <param name="z">z回転</param>
	/// <returns>回転行列</returns>
	Matrix RotationMatrix(float x, float y, float z);

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <param name="vec">x,y,z回転</param>
	/// <returns>回転行列</returns>
	Matrix RotationMatrix(const Vector3& vec);

	/// <summary>
	/// x軸の回転行列の取得
	/// </summary>
	/// <param name="x">x回転</param>
	/// <returns>回転行列</returns>
	Matrix RotationMatrixAxisX(float x);

	/// <summary>
	/// y軸の回転行列の取得
	/// </summary>
	/// <param name="y">y回転</param>
	/// <returns>回転行列</returns>
	Matrix RotationMatrixAxisY(float y);

	/// <summary>
	/// z軸の回転行列の取得
	/// </summary>
	/// <param name="z">z回転</param>
	/// <returns>回転行列</returns>
	Matrix RotationMatrixAxisZ(float z);

	/// <summary>
	/// 移動行列の取得
	/// </summary>
	/// <param name="x">x移動量</param>
	/// <param name="y">y移動量</param>
	/// <param name="z">z移動量</param>
	/// <returns>移動行列</returns>
	Matrix TranslationMatrix(float x, float y, float z);

	/// <summary>
	/// 移動行列の取得
	/// </summary>
	/// <param name="vec">x,y,z移動量</param>
	/// <returns>移動行列</returns>
	Matrix TranslationMatrix(const Vector3& vec);

	/// <summary>
	/// 単位行列の取得
	/// </summary>
	/// <returns>単位行列</returns>
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

		/// <summary>
		/// 逆クォータニオンの取得
		/// </summary>
		/// <returns>逆クォータニオン</returns>
		Quaternion Inverse(void);

		/// <summary>
		/// ベクトルを回転させる
		/// </summary>
		/// <param name="vec">ベクトル</param>
		/// <returns>回転後のベクトル</returns>
		Vector3 RotateVector(const Vector3& vec);

		/// <summary>
		/// 回転行列の取得
		/// </summary>
		/// <returns>回転行列</returns>
		Matrix GetRotationMatrix(void);

		/// <summary>
		/// 回転行列からクォータニオンを生成
		/// </summary>
		/// <param name="mat">回転行列</param>
		void SetRotationMatrix(const Matrix& mat);

		/// <summary>
		/// オイラー角からクォータニオンを生成
		/// </summary>
		/// <param name="angle">オイラー角</param>
		void SetRotationEulerAngle(const Vector3& angle);

		/// <summary>
		/// オイラー角からクォータニオンを生成
		/// </summary>
		/// <param name="angx">x回転</param>
		/// <param name="angy">y回転</param>
		/// <param name="angz">z回転</param>
		void SetRotationEulerAngle(float angx, float angy, float angz);

		/// <summary>
		/// 回転軸と回転量からクォータニオンを生成
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rotateAngle">回転量</param>
		void SetRotationAxis(const Vector3& axis, float rotateAngle);

		/// <summary>
		/// 回転軸と回転量からクォータニオンを生成
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rotateAngle">回転量</param>
		/// <returns>対応するクォータニオン</returns>
		static Quaternion RotateAxis(const Vector3& axis, float rotateAngle);

		/// <summary>
		/// オイラー角からクォータニオンを生成
		/// </summary>
		/// <param name="angx">x回転</param>
		/// <param name="angy">y回転</param>
		/// <param name="angz">z回転</param>
		/// <returns>対応するクォータニオン</returns>
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
