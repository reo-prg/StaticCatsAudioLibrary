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
	/// <para>�T�E���h�N���X</para>
	/// <para>�����Đ�������ł���N���X�ł�</para>
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

		~Sound();
		
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
		/// <para>�Đ��͈͂�ݒ肷��</para>
		/// <para>�Đ��O�ɌĂ΂Ȃ��Ƃ����Ȃ�</para>
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
		/// <para>���[�v�񐔂�ݒ肷��</para>
		/// <para>XAUDIO2_LOOP_INFINITE�Ŗ������[�v</para>
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

		/// <summary>
		/// ���݂̐i�x���擾����
		/// </summary>
		/// <returns>�i�x(0.0 - 1.0)</returns>
		float GetProgress(void);

		/// <summary>
		/// �Đ����̉����~������
		/// </summary>
		/// <returns>����������true</returns>
		bool Stop(void);

		/// <summary>
		/// <para>�������Ă���T�E���h�̃f�[�^��j������</para>
		/// <para>�ĂэĐ��������ꍇ��Load�œǂݍ��݂Ȃ����K�v������</para>
		/// </summary>
		void Destroy(void);

		/// <summary>
		/// ���ݍĐ��\�����擾����
		/// </summary>
		/// <returns>�Đ��\�Ȃ�true</returns>
		bool IsActivated(void) { return activated_; }

		class Sound_Impl;
	protected:
		std::unique_ptr<Sound_Impl> impl_;

		bool activated_ = false;
	public:
		/// <summary>
		/// <para>���̃{�C�X���擾����</para>
		/// <para>�ʏ�͎g�p����K�v�Ȃ�</para>
		/// </summary>
		/// <returns>�\�[�X�{�C�X�̃|�C���^</returns>
		IXAudio2SourceVoice*& GetVoiceAddress(void);

		/// <summary>
		/// <para>�G�~�b�^�[�ɃT�E���h�̃f�[�^��n��</para>
		/// <para>�V�X�e���p�̊֐��Ȃ̂Ŏg�p���Ȃ��ł�������</para>
		/// </summary>
		/// <param name="emitter">�G�~�b�^�[�̃|�C���^</param>
		void GetSoundInnerData(SoundEmitter* emitter);
	};

	/// <summary>
	/// <para>�T�E���h�G�~�b�^�[�N���X</para>
	/// <para>Sound�N���X��3D�v�Z�Ɏg�p�ł��܂�</para>
	/// </summary>
	class SoundEmitter
	{
	public:
		/// <summary>
		/// Sound��ݒ肵�Ȃ���̃G�~�b�^�[�𐶐����܂�
		/// </summary>
		SoundEmitter();

		/// <summary>
		/// Sound���ݒ肳�ꂽ�G�~�b�^�[�𐶐����܂�
		/// </summary>
		/// <param name="sound">�ݒ肵����Sound</param>
		SoundEmitter(Sound* sound);
		~SoundEmitter();

		/// <summary>
		/// Sound��ݒ肷��
		/// </summary>
		/// <param name="sound">�ݒ肵����Sound</param>
		void SetSound(Sound* sound);

		/// <summary>
		/// ���W��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetPosition(const Vector3& pos);

		/// <summary>
		/// ���Ɏw�������������邩
		/// </summary>
		/// <param name="flag">��������Ȃ�true</param>
		void EnableSoundCone(bool flag);

		/// <summary>
		/// ���̎w�����̉~����ݒ肷��
		/// </summary>
		/// <param name="cone">�~��</param>
		void SetSoundCone(const X3DAUDIO_CONE& cone = X3DAudioDefault_DirectionalCone);

		/// <summary>
		/// 3D�̃h�b�v���[���ʂ�L���ɂ��邩
		/// </summary>
		/// <param name="flag">�L���ɂ���Ȃ�true</param>
		void Enable3DDoppler(bool flag);

		/// <summary>
		/// 3D�̃��o�[�u���ʂ�L���ɂ��邩
		/// </summary>
		/// <param name="flag">�L���ɂ���Ȃ�true</param>
		void Enable3DReverb(bool flag);

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
		/// <para>�T�E���h�f�[�^���擾����</para>
		/// <para>�V�X�e���p�Ȃ̂Ŏg�p���Ȃ��ł�������</para>
		/// </summary>
		/// <param name="data">Sound�̓����N���X</param>
		void SetSoundInnerData(Sound::Sound_Impl* data);

		/// <summary>
		/// <para>3D�T�E���h���v�Z����</para>
		/// <para>�V�X�e�����ŌĂяo���̂Ŏg�p���Ȃ��ł�������</para>
		/// </summary>
		/// <param name="handle">X3DAudio�̃n���h��</param>
		/// <param name="listener">���X�i�[</param>
		void Calculate(const X3DAUDIO_HANDLE& handle, X3DAUDIO_LISTENER* listener);
	private:
		class Emitter_Impl;
		std::unique_ptr<Emitter_Impl> impl_;
	};

}
