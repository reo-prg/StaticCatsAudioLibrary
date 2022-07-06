#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <memory>
#include "SCAL_Defines.h"

namespace scal
{
	constexpr unsigned int rootSubmixProcessingStage = 128u;

	class Sound;

	/// <summary>
	/// <para>�m�[�h�N���X</para>
	/// <para>�T�E���h���o�͂����܂ł̒��p�n�_�Ƃ��Ďg�p�ł��A</para>
	/// <para>�ʉ߂���T�E���h�ɉ��ʕύX��G�t�F�N�g�������邱�Ƃ��ł���</para>
	/// </summary>
	class Node
	{
	public:
		Node();
		~Node();

		/// <summary>
		/// <para>���ʂ�ݒ肷��</para>
		/// <para>��) �T�E���h�̉���:0.5 �m�[�h�̉���:0.3</para>
		/// <para>�ŏI�I�ȉ��� = 0.5 * 0.3 = 0.15</para>
		/// </summary>
		/// <param name="volume">����</param>
		void SetVolume(float volume);

		/// <summary>
		/// �o�͐�̃m�[�h��ǉ�����
		/// </summary>
		/// <param name="node">�o�͐�̃m�[�h</param>
		void AddOutputNode(Node* node);

		/// <summary>
		/// �o�͐�̃m�[�h�����O����
		/// </summary>
		/// <param name="node">���O�������m�[�h</param>
		void RemoveOutputNode(Node* node);

		/// <summary>
		///	���͂̃T�E���h��ǉ�����
		/// </summary>
		/// <param name="sound">���͂������T�E���h</param>
		/// <param name="system_value_isCallingAnotherFunc">true�ɂ��Ă�������</param>
		void AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);

		/// <summary>
		///	���͂̃T�E���h�����O����
		/// </summary>
		/// <param name="sound">���O�������T�E���h</param>
		/// <param name="system_value_isCallingAnotherFunc">true�ɂ��Ă�������</param>
		void RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);

		/// <summary>
		/// �G�t�F�N�g��ǉ�����
		/// </summary>
		/// <param name="type">�G�t�F�N�g�̎��</param>
		/// <param name="active">�L���ɂ��邩</param>
		/// <param name="insertPosition">�}������C���f�b�N�X �f�t�H���g���ƈ�Ԍ��</param>
		/// <returns>�}�����ꂽ�C���f�b�N�X</returns>
		int AddEffect(AudioEffectType type, bool active, int insertPosition = -1);

		/// <summary>
		/// �G�t�F�N�g�����O����
		/// </summary>
		/// <param name="position">���O�������C���f�b�N�X</param>
		/// <returns>����������true</returns>
		bool RemoveEffect(int position = -1);
		
		/// <summary>
		/// �G�t�F�N�g��L���ɂ���
		/// </summary>
		/// <param name="position">�L���ɂ���G�t�F�N�g�̃C���f�b�N�X �f�t�H���g���ƑS��</param>
		/// <returns>����������true</returns>
		bool EnableEffect(int position = -1);

		/// <summary>
		/// �G�t�F�N�g�𖳌��ɂ���
		/// </summary>
		/// <param name="position">�����ɂ���G�t�F�N�g�̃C���f�b�N�X �f�t�H���g���ƑS��</param>
		/// <returns>����������true</returns>
		bool DisableEffect(int position = -1);


		/// <summary>
		/// ���o�[�u�̃G�t�F�N�g�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="param">�p�����[�^�[</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex = -1);

		/// <summary>
		/// ���o�[�u�̃G�t�F�N�g�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="param">�p�����[�^�[</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex = -1);

		/// <summary>
		///	�G�R�[�̃G�t�F�N�g�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="feedbackVolume">�o�͂̋���</param>
		/// <param name="delay">�f�B���C</param>
		/// <param name="effectStrength">�G�R�[�̂�����</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetEchoParameter(float strength, float delay, float reverb, int effectIndex = -1);

		/// <summary>
		/// �C�R���C�U�[�̃G�t�F�N�g�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="param">�p�����[�^�[</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex = -1);

		/// <summary>
		/// �}�X�^�����O���~�b�^�[�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="release">loudness����������ۂ�(1 - 20)</param>
		/// <param name="loudness">�ő剹��(1 - 1800)</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex = -1);

		/// <summary>
		/// FX���o�[�u�̃p�����[�^�[��ݒ肷��
		/// </summary>
		/// <param name="diffuse">�ǂ̍d���E�g�U�(0.0 - 1.0)</param>
		/// <param name="roomsize">�����̍L��(0.0001 - 1.0)</param>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);

		/// <summary>
		/// �{�����[�����[�^�[���擾����
		/// </summary>
		/// <param name="effectIndex">�G�t�F�N�g�̃C���f�b�N�X</param>
		/// <returns>�{�����[�����[�^�[</returns>
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);
		
		/// <summary>
		/// <para>�Ή�����G�t�F�N�g�̃C���f�b�N�X���擾����</para>
		/// <para>�C���f�b�N�X���K�v�Ȋ֐���-1��n���Ƃ��̊֐������ŌĂ΂��</para>
		/// </summary>
		/// <param name="type">�G�t�F�N�g�̎��</param>
		/// <returns>�C���f�b�N�X</returns>
		int FindEffectIndex(AudioEffectType type);

		/// <summary>
		/// �t�B���^�[��������
		/// </summary>
		/// <param name="type">�t�B���^�[�̎��</param>
		/// <param name="frequency">���g�� �T���v�����[�g��6�Ŋ������l��1.0�ɑΉ�����(0.0 - 1.0)</param>
		/// <param name="danping">���g���𒴂����ۂ̌����̑傫��(0.0 - 1.5)</param>
		/// <returns>����������true</returns>
		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		bool Create(void);

		void Destroy(void);

		IXAudio2SubmixVoice*& GetVoiceAddress(void);

		bool IsActivated(void) { return activated_; }
	private:
		class Node_Impl;
		std::unique_ptr<Node_Impl> impl_;
		friend Node_Impl;

		bool activated_ = false;
	};
}
