#pragma once
#include "SCAL_Defines.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <x3daudio.h>
#include <memory>
#include <string>
#include "SCAL_Math.h"

namespace scal
{
	class Node;
	class SoundEmitter;

	/// <summary>
	/// Sound�N���X
	/// �����Đ�������ł���N���X�ł�
	/// </summary>
	class Sound
	{
	public:
		Sound();

		/// <summary>
		/// �����Ɠ����ɃT�E���h��ǂݍ���
		/// </summary>
		/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
		Sound(const std::string& filepath);
		virtual ~Sound();
		
		/// <summary>
		/// �T�E���h����荞��
		/// </summary>
		/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
		/// <returns>����������true</returns>
		bool Load(const std::string& filepath);

		/// <summary>
		/// �Đ�����
		/// </summary>
		/// <returns>����������true</returns>
		bool Play(void);
		

		/// <summary>
		/// �Đ��͈͂�ݒ肷��
		/// �Đ��O�ɌĂ΂Ȃ��Ƃ����Ȃ�
		/// </summary>
		/// <param name="begin">�Đ��J�n�n�_(�b)</param>
		/// <param name="length">�Đ��͈͂̒���(�b)</param>
		void SetPlaySegment(float begin = 0.0f, float length = 0.0f);

		/// <summary>
		/// ���[�v�͈͂�ݒ肷��
		/// </summary>
		/// <param name="begin">���[�v�J�n�n�_(�b)</param>
		/// <param name="length">���[�v�͈͂̒���(�b)</param>
		void SetLoopSegment(float begin = 0.0f, float length = 0.0f);

		/// <summary>
		/// ���[�v�񐔂�ݒ肷��
		/// XAUDIO2_LOOP_INFINITE �͖������[�v
		/// </summary>
		/// <param name="count">��</param>
		void SetLoopCount(UINT32 count = XAUDIO2_LOOP_INFINITE);
		
		/// <summary>
		/// ���ʂ�ݒ肷��
		/// </summary>
		/// <param name="volume">���� 1�͌��̉��� 0�͖���</param>
		void SetVolume(float volume = 1.0f);


		/// <summary>
		/// �o�͐�̃m�[�h��ǉ�����
		/// </summary>
		/// <param name="node">�o�͐�̃m�[�h</param>
		/// <param name="system_value_isCallingAnotherFunc">true�ɂ��Ă�������</param>
		void AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);

		/// <summary>
		/// �o�͐�̃m�[�h�����O����
		/// </summary>
		/// <param name="node">���O�������m�[�h</param>
		/// <param name="system_value_isCallingAnotherFunc">true�ɂ��Ă�������</param>
		void RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);


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
		void SetEchoParameter(float feedbackVolume, float delay, float effectStrength, int effectIndex = -1);
		
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
		void SetMasteringLimiterParameter(UINT32 release = FXMASTERINGLIMITER_DEFAULT_RELEASE, 
			UINT32 loudness = FXMASTERINGLIMITER_MAX_LOUDNESS, int effectIndex = -1);
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);

		int FindEffectIndex(AudioEffectType type);


		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		float GetProgress(void);

		bool Stop(void);
		void Destroy(void);

		IXAudio2SourceVoice*& GetVoiceAddress(void);

		bool IsActivated(void) { return activated_; }

		class Sound_Impl;
	protected:
		std::unique_ptr<Sound_Impl> impl_;

		bool activated_ = false;
	public:

		void GetSoundInnerData(SoundEmitter* emitter);
	};


	class SoundEmitter
	{
	public:
		SoundEmitter();
		SoundEmitter(Sound* sound);
		~SoundEmitter();

		void SetSound(Sound* sound);

		void SetPosition(const Vector3& pos);

		void EnableSoundCone(bool flag);
		void SetSoundCone(const X3DAUDIO_CONE& cone = X3DAudioDefault_DirectionalCone);

		void Enable3DDoppler(bool flag);

		void Enable3DReverb(bool flag);

		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		void SetRotate(const Vector3& axis, float rot);
		void SetRotate(float x, float y, float z);
		void SetRotateAsQuaternion(const Quaternion& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const Quaternion& quat);

		Quaternion& GetRotateQuaternionRef(void);

		void EnableVelocity(bool enable = true);

		void UpdateVelocity(void);


		void SetSoundInnerData(Sound::Sound_Impl* data);

		void Calculate(const X3DAUDIO_HANDLE& handle, X3DAUDIO_LISTENER* listener);
	private:
		class Emitter_Impl;
		std::unique_ptr<Emitter_Impl> impl_;
	};

}
