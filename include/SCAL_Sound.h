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
	/// Soundクラス
	/// 音を再生したりできるクラスです
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
		virtual ~Sound();
		
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
		/// 再生範囲を設定する
		/// 再生前に呼ばないといけない
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
		/// ループ回数を設定する
		/// XAUDIO2_LOOP_INFINITE は無限ループ
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
