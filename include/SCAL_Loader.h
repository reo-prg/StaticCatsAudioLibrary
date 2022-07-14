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
		/// wavデータを読み込む
		/// </summary>
		/// <param name="filename">ファイルパス</param>
		/// <returns>成功したらtrue</returns>
		bool LoadWAVFile(const std::string& filename);

		bool LoadWAVFile(unsigned char* rawdata, unsigned int size, const std::string& key);

		/// <summary>
		/// wavデータを取得する
		/// </summary>
		/// <param name="filename">ファイルパス</param>
		/// <returns>wavデータ</returns>
		WAVData* GetWAVFile(const std::string& filename);

		/// <summary>
		/// 読み込んだwavデータを削除する
		/// </summary>
		/// <param name="filename">ファイルパス</param>
		void DestroyWAVFile(const std::string& filename);

		/// <summary>
		/// 読み込んである全てのwavデータを削除する
		/// </summary>
		void Terminate(void);
	private:
		std::unordered_map<std::string, WAVData> wav_;

		static constexpr char fmttag[4] = { 'f', 'm', 't', ' ' };
		static constexpr char datatag[4] = { 'd', 'a', 't', 'a' };
	};
}
