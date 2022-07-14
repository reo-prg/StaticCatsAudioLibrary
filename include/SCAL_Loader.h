#pragma once
#include <unordered_map>
#include <Windows.h>
#include <xaudio2.h>
#include <string>

namespace scal
{
	// WavLoader--------------------------------------------
	struct FmtDesc
	{
		unsigned int chunkSize_;
		unsigned short formatType_;
		unsigned short channel_;
		unsigned int samplesPerSec_;
		unsigned int bytePerSec_;
		unsigned short blockAlign_;
		unsigned short bitPerSample_;
	};

	struct WAVData
	{
		unsigned int fileSize_;
		FmtDesc fmt_;
		unsigned int dataSize_;
		unsigned char* data_;
	};

	class WAVLoader
	{
	public:
		WAVLoader();
		~WAVLoader();

		/// <summary>
		/// wav�f�[�^��ǂݍ���
		/// </summary>
		/// <param name="filename">�t�@�C���p�X</param>
		/// <returns>����������true</returns>
		bool LoadWAVFile(const std::string& filename);

		bool LoadWAVFile(unsigned char* rawdata, unsigned int size, const std::string& key);

		/// <summary>
		/// wav�f�[�^���擾����
		/// </summary>
		/// <param name="filename">�t�@�C���p�X</param>
		/// <returns>wav�f�[�^</returns>
		WAVData* GetWAVFile(const std::string& filename);

		/// <summary>
		/// �ǂݍ���wav�f�[�^���폜����
		/// </summary>
		/// <param name="filename">�t�@�C���p�X</param>
		void DestroyWAVFile(const std::string& filename);

		/// <summary>
		/// �ǂݍ���ł���S�Ă�wav�f�[�^���폜����
		/// </summary>
		void Terminate(void);
	private:
		std::unordered_map<std::string, WAVData> wav_;

		static constexpr char fmttag[4] = { 'f', 'm', 't', ' ' };
		static constexpr char datatag[4] = { 'd', 'a', 't', 'a' };
	};
}
