#pragma once
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

	
	struct SoundData
	{
		unsigned int fileSize_;
		FmtDesc fmt_;
		unsigned int dataSize_;
		unsigned char* data_;
	};

	class AudioManager;
	class WAVLoader;


	/// <summary>
	/// ���C�u�����̃V�X�e��������������
	/// ���̃��C�u�������g�p����O�ɌĂяo���K�v����
	/// </summary>
	/// <returns>����������true</returns>
	bool Initialize(void);

	/// <summary>
	/// ���C�u�������I������
	/// �Ăяo������͂��̃��C�u�����̊֐���N���X���g��Ȃ��ł�������
	/// </summary>
	void Terminate(void);

	/// <summary>
	/// 3D�̃T�E���h���g�p����
	/// Initialize�֐��̑O�ɌĂяo���Ă�������
	/// �f�t�H���g�ł͎g�p���Ȃ��ݒ�ɂȂ��Ă��܂�
	/// �g�p���Ȃ��������ׂ��Ⴂ�ł��B
	/// </summary>
	/// <param name="using3DSound">3D�T�E���h���g�p����Ȃ�true</param>
	void SetUse3DSound(bool using3DSound);



	/// <summary>
	/// �T�E���h��ǂݍ���
	/// Sound�ōĐ�����f�[�^�͂��̊֐��ł��炩���ߓǂݍ���ł�������
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	/// <returns>����������true</returns>
	bool LoadSoundFile(const std::string& filepath);

	/// <summary>
	/// �T�E���h�̐��̃f�[�^���擾����
	/// ���C�u�����p�̊֐��Ȃ̂Ŏg�p���Ȃ��Ă���薳���ł�
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	/// <returns>�T�E���h�f�[�^</returns>
	SoundData GetSoundData(const std::string& filepath);

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�f�[�^��j������
	/// �j�������f�[�^�͍ēx�ǂݍ��ނ܂Ŏg�p�ł��܂���
	/// </summary>
	/// <param name="filepath">�T�E���h�f�[�^�̃t�@�C���p�X</param>
	void DestroySoundFile(const std::string& filepath);


	/// <summary>
	/// WAV�̃��[�_�[���擾����
	/// ���C�u�����p�̊֐��ł����A������WAV�t�@�C���̃��[�_�[�Ƃ���
	/// �g�p���邱�Ƃ͂ł��܂��B
	/// </summary>
	/// <returns>WAV���[�_�[</returns>
	WAVLoader& GetWavLoader(void);

	/// <summary>
	/// ���̃��C�u������IXAudio2�I�u�W�F�N�g���擾����
	/// �A�h���X�̏��������Ȃǂ̑���͂��Ȃ��ł�������
	/// IXAudio2�̊֐��𒼐ڌĂяo���������p
	/// </summary>
	/// <returns>IXAudio2�I�u�W�F�N�g</returns>
	IXAudio2* GetXAudio2Core(void);

	/// <summary>
	/// �}�X�^�����O�{�C�X�̏����擾����
	/// �`�����l������T���v�����[�g�Ȃǂ��擾�ł��܂�
	/// </summary>
	/// <returns>�}�X�^�����O�{�C�X�̏��</returns>
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void);
}
