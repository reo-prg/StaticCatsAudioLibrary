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
	/// <para>サウンドクラス</para>
	/// <para>音を再生したりできるクラスです</para>
	/// </summary>
	class Sound
	{
	public:
		Sound();

		/// <summary>
		/// 生成と同時にサウンドを読み込む
		/// </summary>
		/// <param name="filepath">サウンドデータのファイルパス</param>
		Sound(const std::string& filepath);

		~Sound();
		
		/// <summary>
		/// サウンドを取り込む
		/// </summary>
		/// <param name="filepath">サウンドデータのファイルパス</param>
		/// <returns>成功したらtrue</returns>
		bool Load(const std::string& filepath);

		/// <summary>
		/// 再生する
		/// </summary>
		/// <returns>成功したらtrue</returns>
		bool Play(void);
		

		/// <summary>
		/// <para>再生範囲を設定する</para>
		/// <para>再生前に呼ばないといけない</para>
		/// </summary>
		/// <param name="begin">再生開始地点(秒)</param>
		/// <param name="length">再生範囲の長さ(秒)</param>
		void SetPlaySegment(float begin = 0.0f, float length = 0.0f);

		/// <summary>
		/// ループ範囲を設定する
		/// </summary>
		/// <param name="begin">ループ開始地点(秒)</param>
		/// <param name="length">ループ範囲の長さ(秒)</param>
		void SetLoopSegment(float begin = 0.0f, float length = 0.0f);

		/// <summary>
		/// <para>ループ回数を設定する</para>
		/// <para>XAUDIO2_LOOP_INFINITEで無限ループ</para>
		/// </summary>
		/// <param name="count">回数</param>
		void SetLoopCount(UINT32 count = XAUDIO2_LOOP_INFINITE);
		
		/// <summary>
		/// 音量を設定する
		/// </summary>
		/// <param name="volume">音量 1は元の音量 0は無音</param>
		void SetVolume(float volume = 1.0f);


		/// <summary>
		/// 出力先のノードを追加する
		/// </summary>
		/// <param name="node">出力先のノード</param>
		/// <param name="system_value_isCallingAnotherFunc">trueにしてください</param>
		void AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);

		/// <summary>
		/// 出力先のノードを除外する
		/// </summary>
		/// <param name="node">除外したいノード</param>
		/// <param name="system_value_isCallingAnotherFunc">trueにしてください</param>
		void RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);


		/// <summary>
		/// エフェクトを追加する
		/// </summary>
		/// <param name="type">エフェクトの種類</param>
		/// <param name="active">有効にするか</param>
		/// <param name="insertPosition">挿入するインデックス デフォルトだと一番後ろ</param>
		/// <returns>挿入されたインデックス</returns>
		int AddEffect(AudioEffectType type, bool active, int insertPosition = -1);

		/// <summary>
		/// エフェクトを除外する
		/// </summary>
		/// <param name="position">除外したいインデックス</param>
		/// <returns>成功したらtrue</returns>
		bool RemoveEffect(int position = -1);

		/// <summary>
		/// エフェクトを有効にする
		/// </summary>
		/// <param name="position">有効にするエフェクトのインデックス デフォルトだと全て</param>
		/// <returns>成功したらtrue</returns>
		bool EnableEffect(int position = -1);

		/// <summary>
		/// エフェクトを無効にする
		/// </summary>
		/// <param name="position">無効にするエフェクトのインデックス デフォルトだと全て</param>
		/// <returns>成功したらtrue</returns>
		bool DisableEffect(int position = -1);

		
		/// <summary>
		/// リバーブのエフェクトのパラメーターを設定する
		/// </summary>
		/// <param name="param">パラメーター</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex = -1);

		/// <summary>
		/// リバーブのエフェクトのパラメーターを設定する
		/// </summary>
		/// <param name="param">パラメーター</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex = -1);
		
		/// <summary>
		///	エコーのエフェクトのパラメーターを設定する
		/// </summary>
		/// <param name="feedbackVolume">出力の強さ</param>
		/// <param name="delay">ディレイ</param>
		/// <param name="effectStrength">エコーのかかり具合</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetEchoParameter(float feedbackVolume, float delay, float effectStrength, int effectIndex = -1);
		
		/// <summary>
		/// イコライザーのエフェクトのパラメーターを設定する
		/// </summary>
		/// <param name="param">パラメーター</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex = -1);
		
		/// <summary>
		/// マスタリングリミッターのパラメーターを設定する
		/// </summary>
		/// <param name="release">loudnessを下回った際の(1 - 20)</param>
		/// <param name="loudness">最大音量(1 - 1800)</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetMasteringLimiterParameter(UINT32 release = FXMASTERINGLIMITER_DEFAULT_RELEASE, 
			UINT32 loudness = FXMASTERINGLIMITER_MAX_LOUDNESS, int effectIndex = -1);

		/// <summary>
		/// FXリバーブのパラメーターを設定する
		/// </summary>
		/// <param name="diffuse">壁の硬さ・拡散具合(0.0 - 1.0)</param>
		/// <param name="roomsize">部屋の広さ(0.0001 - 1.0)</param>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);

		/// <summary>
		/// ボリュームメーターを取得する
		/// </summary>
		/// <param name="effectIndex">エフェクトのインデックス</param>
		/// <returns>ボリュームメーター</returns>
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);

		/// <summary>
		/// <para>対応するエフェクトのインデックスを取得する</para>
		/// <para>インデックスが必要な関数に-1を渡すとこの関数が中で呼ばれる</para>
		/// </summary>
		/// <param name="type">エフェクトの種類</param>
		/// <returns>インデックス</returns>
		int FindEffectIndex(AudioEffectType type);

		/// <summary>
		/// フィルターをかける
		/// </summary>
		/// <param name="type">フィルターの種類</param>
		/// <param name="frequency">周波数 サンプルレートを6で割った値が1.0に対応する(0.0 - 1.0)</param>
		/// <param name="danping">周波数を超えた際の減衰の大きさ(0.0 - 1.5)</param>
		/// <returns>成功したらtrue</returns>
		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		/// <summary>
		/// 現在の進度を取得する
		/// </summary>
		/// <returns>進度(0.0 - 1.0)</returns>
		float GetProgress(void);

		/// <summary>
		/// 再生中の音を停止させる
		/// </summary>
		/// <returns>成功したらtrue</returns>
		bool Stop(void);

		/// <summary>
		/// <para>所持しているサウンドのデータを破棄する</para>
		/// <para>再び再生したい場合はLoadで読み込みなおす必要がある</para>
		/// </summary>
		void Destroy(void);

		/// <summary>
		/// 現在再生可能かを取得する
		/// </summary>
		/// <returns>再生可能ならtrue</returns>
		bool IsActivated(void) { return activated_; }

		class Sound_Impl;
	protected:
		std::unique_ptr<Sound_Impl> impl_;

		bool activated_ = false;
	public:
		/// <summary>
		/// <para>生のボイスを取得する</para>
		/// <para>通常は使用する必要なし</para>
		/// </summary>
		/// <returns>ソースボイスのポインタ</returns>
		IXAudio2SourceVoice*& GetVoiceAddress(void);

		/// <summary>
		/// <para>エミッターにサウンドのデータを渡す</para>
		/// <para>システム用の関数なので使用しないでください</para>
		/// </summary>
		/// <param name="emitter">エミッターのポインタ</param>
		void GetSoundInnerData(SoundEmitter* emitter);
	};

	/// <summary>
	/// <para>サウンドエミッタークラス</para>
	/// <para>Soundクラスを3D計算に使用できます</para>
	/// </summary>
	class SoundEmitter
	{
	public:
		/// <summary>
		/// Soundを設定しない空のエミッターを生成します
		/// </summary>
		SoundEmitter();

		/// <summary>
		/// Soundが設定されたエミッターを生成します
		/// </summary>
		/// <param name="sound">設定したいSound</param>
		SoundEmitter(Sound* sound);
		~SoundEmitter();

		/// <summary>
		/// Soundを設定する
		/// </summary>
		/// <param name="sound">設定したいSound</param>
		void SetSound(Sound* sound);

		/// <summary>
		/// 座標を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(const Vector3& pos);

		/// <summary>
		/// 音に指向性を持たせるか
		/// </summary>
		/// <param name="flag">持たせるならtrue</param>
		void EnableSoundCone(bool flag);

		/// <summary>
		/// 音の指向性の円錐を設定する
		/// </summary>
		/// <param name="cone">円錐</param>
		void SetSoundCone(const X3DAUDIO_CONE& cone = X3DAudioDefault_DirectionalCone);

		/// <summary>
		/// 3Dのドップラー効果を有効にするか
		/// </summary>
		/// <param name="flag">有効にするならtrue</param>
		void Enable3DDoppler(bool flag);

		/// <summary>
		/// 3Dのリバーブ効果を有効にするか
		/// </summary>
		/// <param name="flag">有効にするならtrue</param>
		void Enable3DReverb(bool flag);

		/// <summary>
		/// <para>デフォルトの向きを設定する</para>
		/// <para>正面と上は直角である必要あり</para>
		/// </summary>
		/// <param name="front">正面</param>
		/// <param name="up">上</param>
		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rot">回転角度</param>
		void SetRotate(const Vector3& axis, float rot);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="x">x回転</param>
		/// <param name="y">y回転</param>
		/// <param name="z">z回転</param>
		void SetRotate(float x, float y, float z);

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="quat">クォータニオン</param>
		void SetRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// 回転を加算する
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="rot">回転角度</param>
		void AddRotate(const Vector3& axis, float rot);

		/// <summary>
		/// 回転を加算する
		/// </summary>
		/// <param name="quat">クォータニオン</param>
		void AddRotateAsQuaternion(const Quaternion& quat);

		/// <summary>
		/// 回転を取得する
		/// </summary>
		/// <returns>回転用のクォータニオンの参照</returns>
		Quaternion& GetRotateQuaternionRef(void);

		/// <summary>
		/// 速度を有効にするか
		/// </summary>
		/// <param name="enable">有効にするならtrue</param>
		void EnableVelocity(bool enable = true);

		/// <summary>
		/// 速度を更新する
		/// </summary>
		void UpdateVelocity(void);

		/// <summary>
		/// <para>サウンドデータを取得する</para>
		/// <para>システム用なので使用しないでください</para>
		/// </summary>
		/// <param name="data">Soundの内部クラス</param>
		void SetSoundInnerData(Sound::Sound_Impl* data);

		/// <summary>
		/// <para>3Dサウンドを計算する</para>
		/// <para>システム側で呼び出すので使用しないでください</para>
		/// </summary>
		/// <param name="handle">X3DAudioのハンドル</param>
		/// <param name="listener">リスナー</param>
		void Calculate(const X3DAUDIO_HANDLE& handle, X3DAUDIO_LISTENER* listener);
	private:
		class Emitter_Impl;
		std::unique_ptr<Emitter_Impl> impl_;
	};

}
