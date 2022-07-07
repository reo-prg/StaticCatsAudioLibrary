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
	/// ����
	/// </summary>
	/// <param name="val1">1�ڂ̒l</param>
	/// <param name="val2">2�ڂ̒l</param>
	/// <returns>val1 � val2</returns>
	float Dot(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// �O��
	/// </summary>
	/// <param name="val1">1�ڂ̒l</param>
	/// <param name="val2">2�ڂ̒l</param>
	/// <returns>val1 �~ val2</returns>
	Vector3 Cross(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// ���K��
	/// </summary>
	/// <param name="val">�x�N�g��</param>
	/// <returns>���K���x�N�g��</returns>
	Vector3 Normalize(const Vector3& val);

	/// <summary>
	/// �x�N�g���̒���
	/// </summary>
	/// <param name="val">�x�N�g��</param>
	/// <returns>����</returns>
	float Length(const Vector3& val);

	/// <summary>
	/// �x�N�g���̒�����2��
	/// </summary>
	/// <param name="val">�x�N�g��</param>
	/// <returns>������2��</returns>
	float SquareLength(const Vector3& val);

	/// <summary>
	/// 2�_�Ԃ̋���
	/// </summary>
	/// <param name="val1">1�ڂ̓_</param>
	/// <param name="val2">2�ڂ̓_</param>
	/// <returns>����</returns>
	float Distance(const Vector3& val1, const Vector3& val2);

	/// <summary>
	/// 2�_�Ԃ̋�����2��
	/// </summary>
	/// <param name="val1">1�ڂ̓_</param>
	/// <param name="val2">2�ڂ̓_</param>
	/// <returns>������2��</returns>
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
	/// ��]�s��̎擾
	/// </summary>
	/// <param name="x">x��]</param>
	/// <param name="y">y��]</param>
	/// <param name="z">z��]</param>
	/// <returns>��]�s��</returns>
	Matrix RotationMatrix(float x, float y, float z);

	/// <summary>
	/// ��]�s��̎擾
	/// </summary>
	/// <param name="vec">x,y,z��]</param>
	/// <returns>��]�s��</returns>
	Matrix RotationMatrix(const Vector3& vec);

	/// <summary>
	/// x���̉�]�s��̎擾
	/// </summary>
	/// <param name="x">x��]</param>
	/// <returns>��]�s��</returns>
	Matrix RotationMatrixAxisX(float x);

	/// <summary>
	/// y���̉�]�s��̎擾
	/// </summary>
	/// <param name="y">y��]</param>
	/// <returns>��]�s��</returns>
	Matrix RotationMatrixAxisY(float y);

	/// <summary>
	/// z���̉�]�s��̎擾
	/// </summary>
	/// <param name="z">z��]</param>
	/// <returns>��]�s��</returns>
	Matrix RotationMatrixAxisZ(float z);

	/// <summary>
	/// �ړ��s��̎擾
	/// </summary>
	/// <param name="x">x�ړ���</param>
	/// <param name="y">y�ړ���</param>
	/// <param name="z">z�ړ���</param>
	/// <returns>�ړ��s��</returns>
	Matrix TranslationMatrix(float x, float y, float z);

	/// <summary>
	/// �ړ��s��̎擾
	/// </summary>
	/// <param name="vec">x,y,z�ړ���</param>
	/// <returns>�ړ��s��</returns>
	Matrix TranslationMatrix(const Vector3& vec);

	/// <summary>
	/// �P�ʍs��̎擾
	/// </summary>
	/// <returns>�P�ʍs��</returns>
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
		/// �t�N�H�[�^�j�I���̎擾
		/// </summary>
		/// <returns>�t�N�H�[�^�j�I��</returns>
		Quaternion Inverse(void);

		/// <summary>
		/// �x�N�g������]������
		/// </summary>
		/// <param name="vec">�x�N�g��</param>
		/// <returns>��]��̃x�N�g��</returns>
		Vector3 RotateVector(const Vector3& vec);

		/// <summary>
		/// ��]�s��̎擾
		/// </summary>
		/// <returns>��]�s��</returns>
		Matrix GetRotationMatrix(void);

		/// <summary>
		/// ��]�s�񂩂�N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="mat">��]�s��</param>
		void SetRotationMatrix(const Matrix& mat);

		/// <summary>
		/// �I�C���[�p����N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="angle">�I�C���[�p</param>
		void SetRotationEulerAngle(const Vector3& angle);

		/// <summary>
		/// �I�C���[�p����N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="angx">x��]</param>
		/// <param name="angy">y��]</param>
		/// <param name="angz">z��]</param>
		void SetRotationEulerAngle(float angx, float angy, float angz);

		/// <summary>
		/// ��]���Ɖ�]�ʂ���N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="axis">��]��</param>
		/// <param name="rotateAngle">��]��</param>
		void SetRotationAxis(const Vector3& axis, float rotateAngle);

		/// <summary>
		/// ��]���Ɖ�]�ʂ���N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="axis">��]��</param>
		/// <param name="rotateAngle">��]��</param>
		/// <returns>�Ή�����N�H�[�^�j�I��</returns>
		static Quaternion RotateAxis(const Vector3& axis, float rotateAngle);

		/// <summary>
		/// �I�C���[�p����N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="angx">x��]</param>
		/// <param name="angy">y��]</param>
		/// <param name="angz">z��]</param>
		/// <returns>�Ή�����N�H�[�^�j�I��</returns>
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
