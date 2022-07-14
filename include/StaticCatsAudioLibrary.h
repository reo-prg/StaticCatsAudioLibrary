#pragma once
#include <Windows.h>
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

	/// <summary>
	/// 音の生データ
	/// </summary>
	struct SoundData
	{
		unsigned int fileSize_;
		FmtDesc fmt_;
		unsigned int dataSize_;
		unsigned char* data_;
	};

	/// <summary>
	/// オイラー角の計算順序
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
	/// <para>ライブラリのシステムを初期化する</para>
	/// <para>このライブラリを使用する前に呼び出す必要あり</para>
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool Initialize(void);

	/// <summary>
	/// <para>ライブラリを終了する</para>
	/// <para>呼び出した後はこのライブラリの関数やクラスを使わないでください</para>
	/// </summary>
	void Terminate(void);

	/// <summary>
	/// <para>3Dのサウンドを使用する</para>
	/// <para>Initialize関数の前に呼び出してください</para>
	/// <para>デフォルトでは使用しない設定になっています</para>
	/// <para>使用しない方が負荷が低いです。</para>
	/// </summary>
	/// <param name="using3DSound">3Dサウンドを使用するならtrue</param>
	void SetUse3DSound(bool using3DSound);



	/// <summary>
	/// <para>サウンドを読み込む</para>
	/// <para>Soundで再生するデータはこの関数であらかじめ読み込んでください</para>
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	/// <returns>成功したらtrue</returns>
	bool LoadSoundFile(const std::string& filepath);

	/// <summary>
	/// <para>サウンドを読み込む</para>
	/// </summary>
	/// <param name="rawdata">生データ</param>
	/// <param name="size">ファイルサイズ</param>
	/// <param name="key">キー</param>
	/// <returns>成功したらtrue</returns>
	bool LoadSoundFile(unsigned char* rawdata, unsigned int size, const std::string& key);

	/// <summary>
	/// <para>サウンドの生のデータを取得する</para>
	/// <para>ライブラリ用の関数なので使用しなくても問題無いです</para>
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	/// <returns>サウンドデータ</returns>
	SoundData GetSoundData(const std::string& filepath);

	/// <summary>
	/// <para>読み込んだサウンドデータを破棄する</para>
	/// <para>破棄したデータは再度読み込むまで使用できません</para>
	/// </summary>
	/// <param name="filepath">サウンドデータのファイルパス</param>
	void DestroySoundFile(const std::string& filepath);

	/// <summary>
	/// エミッターを登録する
	/// </summary>
	/// <param name="emitter">エミッター</param>
	void RegisterEmitter(SoundEmitter* emitter);

	/// <summary>
	/// リスナーを登録する
	/// </summary>
	/// <param name="listener">リスナー</param>
	void RegisterListener(Listener* listener);

	/// <summary>
	/// 3Dサウンドを更新する
	/// </summary>
	/// <param name="delta">経過時間</param>
	void Update3DAudio(float delta);

	/// <summary>
	/// <para>3Dサウンドの更新頻度を設定する</para>
	/// <para>更新は負荷が高いので3から5フレームに1度がおすすめ</para>
	/// <para>デフォルトは0.05秒(60fpsでの3フレーム)</para>
	/// </summary>
	/// <param name="interval">更新間隔(秒)</param>
	void SetUpdateInterval(float interval);

	/// <summary>
	/// <para>WAVのローダーを取得する</para>
	/// <para>ライブラリ用の関数ですが、純粋なWAVファイルのローダーとして</para>
	/// <para>使用することはできます。</para>
	/// </summary>
	/// <returns>WAVローダー</returns>
	WAVLoader& GetWavLoader(void);

	/// <summary>
	/// <para>このライブラリのIXAudio2オブジェクトを取得する</para>
	/// <para>アドレスの書き換えなどの操作はしないでください</para>
	/// <para>IXAudio2の関数を直接呼び出したい方用</para>
	/// </summary>
	/// <returns>IXAudio2オブジェクト</returns>
	IXAudio2* GetXAudio2Core(void);

	/// <summary>
	/// <para>マスタリングボイスの情報を取得する</para>
	/// <para>チャンネル数やサンプルレートなどを取得できます</para>
	/// </summary>
	/// <returns>マスタリングボイスの情報</returns>
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void);

	/// <summary>
	/// <para>マスタリングボイスを取得する</para>
	/// <para>通常は使用する必要なし</para>
	/// </summary>
	/// <returns>マスタリングボイス</returns>
	IXAudio2MasteringVoice*& GetMasterVoice(void);

	void SetEulerOrder(EulerOrder order);

	EulerOrder GetEulerOrder(void);
}
