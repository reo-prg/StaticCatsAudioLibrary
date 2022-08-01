#pragma once
#include <Windows.h>
#include "SCAL_Defines.h"
#include <xaudio2fx.h>

//#pragma comment(lib,"xapobase.lib")

namespace scal
{
	/// <summary>
	/// <para>�T�E���h�G�t�F�N�g�̃N���X</para>
	/// <para>Sound��Node�������ŊǗ����Ă���̂�</para>
	/// <para>�g�p����K�v�Ȃ�</para>
	/// </summary>
	class EffectParameter
	{
	public:
		~EffectParameter();

		/// <summary>
		/// �G�t�F�N�g�̎�ނ��擾����
		/// </summary>
		/// <returns>�G�t�F�N�g�̎��</returns>
		AudioEffectType& GetEffectType(void);

		/// <summary>
		/// ���̃G�t�F�N�g�̃A�h���X���擾����
		/// </summary>
		/// <returns>�G�t�F�N�g�̃A�h���X</returns>
		IUnknown*& GetEffectRawAddress(void);

		/// <summary>
		/// �G�t�F�N�g�̃p�����[�^�[���擾����
		/// </summary>
		/// <returns>�p�����[�^�[</returns>
		void*& GetParameterRawAddress(void);
	private:
		AudioEffectType type_;

		IUnknown* pEffect_;
		void* param_;
	};

	/// <summary>
	/// <para>�G�t�F�N�g�𐶐�����</para>
	/// <para>Sound��Node�������ŌĂяo���̂�</para>
	/// <para>�g�p����K�v�Ȃ�</para>
	/// </summary>
	/// <param name="param">�T�E���h�G�t�F�N�g</param>
	/// <param name="type">�G�t�F�N�g�̎��</param>
	/// <param name="channel">�`�����l����</param>
	void GenerateEffectInstance(EffectParameter& param, AudioEffectType type, unsigned int channel = 0u);
}
