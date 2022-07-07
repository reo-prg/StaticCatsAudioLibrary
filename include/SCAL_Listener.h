#pragma once
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <x3daudio.h>
#include "../include/SCAL_Math.h"

namespace scal
{
	/// <summary>
	/// <para>���X�i�[�N���X</para>
	/// <para>3D�T�E���h�̒������̃N���X�ł�</para>
	/// </summary>
	class Listener
	{
	public:
		Listener();
		~Listener();

		/// <summary>
		/// ���W��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetPosition(const Vector3& pos);

		/// <summary>
		/// <para>�f�t�H���g�̌�����ݒ肷��</para>
		/// <para>���ʂƏ�͒��p�ł���K�v����</para>
		/// </summary>
		/// <param name="front">����</param>
		/// <param name="up">��</param>
		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		/// <summary>
		/// ��]��ݒ肷��
		/// </summary>
		/// <param name="axis">��]��</param>
		/// <param name="rot">��]�p�x</param>
		void SetRotate(const Vector3& axis, float rot);

		/// <summary>
		/// ��]��ݒ肷��
		/// </summary>
		/// <param name="x">x��]</param>
		/// <param name="y">y��]</param>
		/// <param name="z">z��]</param>
		void SetRotate(float x, float y, float z);

		/// <summary>
		/// ��]��ݒ肷��
		/// </summary>
		/// <param name="quat">�N�H�[�^�j�I��</param>
		void SetRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// ��]�����Z����
		/// </summary>
		/// <param name="axis">��]��</param>
		/// <param name="rot">��]�p�x</param>
		void AddRotate(const Vector3& axis, float rot);

		/// <summary>
		/// ��]�����Z����
		/// </summary>
		/// <param name="quat">�N�H�[�^�j�I��</param>
		void AddRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// ��]���擾����
		/// </summary>
		/// <returns>��]�p�̃N�H�[�^�j�I���̎Q��</returns>
		Quaternion& GetRotateQuaternionRef(void);

		/// <summary>
		/// ���x��L���ɂ��邩
		/// </summary>
		/// <param name="enable">�L���ɂ���Ȃ�true</param>
		void EnableVelocity(bool enable = true);

		/// <summary>
		/// ���x���X�V����
		/// </summary>
		void UpdateVelocity(void);

		/// <summary>
		/// <para>���̃��X�i�[���擾����</para>
		/// <para>�ʏ�͎g�p����K�v�Ȃ�</para>
		/// </summary>
		/// <returns>���X�i�[�I�u�W�F�N�g</returns>
		X3DAUDIO_LISTENER& GetRawListener(void);
	private:
		class Listener_Impl;
		std::unique_ptr<Listener_Impl> impl_;
	};

}
