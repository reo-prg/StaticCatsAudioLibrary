#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <string>
#include "SCAL_Loader.h"

#pragma comment(lib,"xaudio2.lib")


namespace scal
{
	/// <summary>
	/// Sound�̍Đ����
	/// </summary>
	enum class SoundState
	{
		Playing,
		Stop,
	};

	/// <summary>
	/// ���̐��f�[�^
	/// </summary>
	struct SoundData
	{
		unsigned int fileSize_;
		FmtDesc fmt_;
		unsigned int dataSize_;
		unsigned char* data_;
	};

	/// <summary>
	/// �I�C���[�p�̌v�Z����
	/// </summary>
	enum class EulerOrder
	{
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

	class AudioManager;
	class WAVLoader;
	class SoundEmitter;
	class Listener;

	/// <summary>
	/// <para>���C�u�����̃V�X�e��������������</para>
	/// <para>���̃��C�u�������g�p����O�ɌĂяo���K�v����</para>
	/// </summary>
	/// <returns>����������true</returns>
	bool Initialize(void);

	/// <summary>
	/// <para>���C�u�������I������</para>
	/// <para>�Ăяo������͂��̃��C�u�����̊֐���N���X���g��Ȃ��ł�������</para>
	/// </summary>
	void Terminate(void);

	/// <summary>
	/// <para>3D�̃T�E���h���g�p����</para>
	/// <para>Initialize�֐��̑O�ɌĂяo���Ă�������</para>
	/// <para>�f�t�H���g�ł͎g�p���Ȃ��ݒ�ɂȂ��Ă��܂�</para>
	/// <para>�g�p���Ȃ��������ׂ��Ⴂ�ł��B</para>
	/// </summary>
	/// <param name="using3DSound">3D�T�E���h���g�p����Ȃ�true</param>
	void SetUse3DSound(bool using3DSound);



	/// <summary>
	/// <para>�T�E���h��ǂݍ���</para>
	/// <para>Sound�ōĐ�����f�[�^�͂��̊֐��ł��炩���ߓǂݍ���ł�������</para>
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	/// <returns>����������true</returns>
	bool LoadSoundFile(const std::string& filepath);

	/// <summary>
	/// <para>�T�E���h��ǂݍ���</para>
	/// </summary>
	/// <param name="rawdata">���f�[�^</param>
	/// <param name="size">�t�@�C���T�C�Y</param>
	/// <param name="key">�L�[</param>
	/// <returns>����������true</returns>
	bool LoadSoundFile(unsigned char* rawdata, unsigned int size, const std::string& key);

	/// <summary>
	/// <para>�T�E���h�̐��̃f�[�^���擾����</para>
	/// <para>���C�u�����p�̊֐��Ȃ̂Ŏg�p���Ȃ��Ă���薳���ł�</para>
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	/// <returns>�T�E���h�f�[�^</returns>
	SoundData GetSoundData(const std::string& filepath);

	/// <summary>
	/// <para>�ǂݍ��񂾃T�E���h�f�[�^��j������</para>
	/// <para>�j�������f�[�^�͍ēx�ǂݍ��ނ܂Ŏg�p�ł��܂���</para>
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	void DestroySoundFile(const std::string& filepath);

	/// <summary>
	/// �G�~�b�^�[��o�^����
	/// </summary>
	/// <param name="emitter">�G�~�b�^�[</param>
	void RegisterEmitter(SoundEmitter* emitter);

	/// <summary>
	/// ���X�i�[��o�^����
	/// </summary>
	/// <param name="listener">���X�i�[</param>
	void RegisterListener(Listener* listener);

	/// <summary>
	/// 3D�T�E���h���X�V����
	/// </summary>
	/// <param name="delta">�o�ߎ���</param>
	void Update3DAudio(float delta);

	/// <summary>
	/// <para>3D�T�E���h�̍X�V�p�x��ݒ肷��</para>
	/// <para>�X�V�͕��ׂ������̂�3����5�t���[����1�x����������</para>
	/// <para>�f�t�H���g��0.05�b(60fps�ł�3�t���[��)</para>
	/// </summary>
	/// <param name="interval">�X�V�Ԋu(�b)</param>
	void SetUpdateInterval(float interval);

	/// <summary>
	/// <para>WAV�̃��[�_�[���擾����</para>
	/// <para>���C�u�����p�̊֐��ł����A������WAV�t�@�C���̃��[�_�[�Ƃ���</para>
	/// <para>�g�p���邱�Ƃ͂ł��܂��B</para>
	/// </summary>
	/// <returns>WAV���[�_�[</returns>
	WAVLoader& GetWavLoader(void);

	/// <summary>
	/// <para>���̃��C�u������IXAudio2�I�u�W�F�N�g���擾����</para>
	/// <para>�A�h���X�̏��������Ȃǂ̑���͂��Ȃ��ł�������</para>
	/// <para>IXAudio2�̊֐��𒼐ڌĂяo���������p</para>
	/// </summary>
	/// <returns>IXAudio2�I�u�W�F�N�g</returns>
	IXAudio2* GetXAudio2Core(void);

	/// <summary>
	/// <para>�}�X�^�����O�{�C�X�̏����擾����</para>
	/// <para>�`�����l������T���v�����[�g�Ȃǂ��擾�ł��܂�</para>
	/// </summary>
	/// <returns>�}�X�^�����O�{�C�X�̏��</returns>
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void);

	/// <summary>
	/// <para>�}�X�^�����O�{�C�X���擾����</para>
	/// <para>�ʏ�͎g�p����K�v�Ȃ�</para>
	/// </summary>
	/// <returns>�}�X�^�����O�{�C�X</returns>
	IXAudio2MasteringVoice*& GetMasterVoice(void);

	void SetEulerOrder(EulerOrder order);

	EulerOrder GetEulerOrder(void);
}
