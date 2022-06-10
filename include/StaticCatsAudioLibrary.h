#pragma once
#include <xaudio2.h>
#include <string>
#include "SCAL_Loader.h"

#pragma comment(lib,"xaudio2.lib")


namespace scal
{
	/// <summary>
	/// Soundの再生状態
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
	/// ライブラリのシステムを初期化する
	/// このライブラリを使用する前に呼び出す必要あり
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool Initialize(void);

	/// <summary>
	/// ライブラリを終了する
	/// 呼び出した後はこのライブラリの関数やクラスを使わないでください
	/// </summary>
	void Terminate(void);

	/// <summary>
	/// 3Dのサウンドを使用する
	/// Initialize関数の前に呼び出してください
	/// デフォルトでは使用しない設定になっています
	/// 使用しない方が負荷が低いです。
	/// </summary>
	/// <param name="using3DSound">3Dサウンドを使用するならtrue</param>
	void SetUse3DSound(bool using3DSound);



	/// <summary>
	/// サウンドを読み込む
	/// Soundで再生するデータはこの関数であらかじめ読み込んでください
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	/// <returns>成功したらtrue</returns>
	bool LoadSoundFile(const std::string& filepath);

	/// <summary>
	/// サウンドの生のデータを取得する
	/// ライブラリ用の関数なので使用しなくても問題無いです
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	/// <returns>サウンドデータ</returns>
	SoundData GetSoundData(const std::string& filepath);

	/// <summary>
	/// 読み込んだサウンドデータを破棄する
	/// 破棄したデータは再度読み込むまで使用できません
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	void DestroySoundFile(const std::string& filepath);


	/// <summary>
	/// WAVのローダーを取得する
	/// ライブラリ用の関数ですが、純粋なWAVファイルのローダーとして
	/// 使用することはできます。
	/// </summary>
	/// <returns>WAVローダー</returns>
	WAVLoader& GetWavLoader(void);

	/// <summary>
	/// このライブラリのIXAudio2オブジェクトを取得する
	/// アドレスの書き換えなどの操作はしないでください
	/// IXAudio2の関数を直接呼び出したい方用
	/// </summary>
	/// <returns>IXAudio2オブジェクト</returns>
	IXAudio2* GetXAudio2Core(void);

	/// <summary>
	/// マスタリングボイスの情報を取得する
	/// チャンネル数やサンプルレートなどを取得できます
	/// </summary>
	/// <returns>マスタリングボイスの情報</returns>
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void);
}
