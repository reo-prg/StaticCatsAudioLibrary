#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <memory>
#include "SCAL_Defines.h"

namespace scal
{
	constexpr unsigned int rootSubmixProcessingStage = 128u;

	class Sound;

	/// <summary>
	/// <para>ノードクラス</para>
	/// <para>サウンドが出力されるまでの中継地点として使用でき、</para>
	/// <para>通過するサウンドに音量変更やエフェクトをかけることができる</para>
	/// <para>使用方法</para>
	/// <para>ゲームの初期化時に使用するノードを全て生成して、</para>
	/// <para>サウンドが通るツリーを構成しましょう</para>
	/// <para>その際、最終出力に近いノードから構築してください</para>
	/// </summary>
	class Node
	{
	public:
		Node();
		~Node();

		/// <summary>
		/// 出力先のノードを追加する
		/// </summary>
		/// <param name="node">出力先のノード</param>
		void AddOutputNode(Node* node);

		/// <summary>
		/// 出力先のノードを除外する
		/// </summary>
		/// <param name="node">除外したいノード</param>
		void RemoveOutputNode(Node* node);

		/// <summary>
		/// <para>このノードを生成する</para>
		/// <para>※出力先のノードを全て登録してから呼んでください</para>
		/// <para>例</para>
		/// <code>Node node;
		/// <para>node.AddOutputNode(another_node);</para>
		/// <para>node.AddOutputNode(some_node);</para>
		/// <para>node.Create();</para></code>
		/// </summary>
		/// <returns>成功したらtrue</returns>
		bool Create(void);

		/// <summary>
		/// <para>音量を設定する</para>
		/// <para>例) サウンドの音量:0.5 ノードの音量:0.3</para>
		/// <para>最終的な音量 = 0.5 * 0.3 = 0.15</para>
		/// </summary>
		/// <param name="volume">音量</param>
		void SetVolume(float volume);

		/// <summary>
		///	入力のサウンドを追加する
		/// </summary>
		/// <param name="sound">入力したいサウンド</param>
		/// <param name="system_value_isCallingAnotherFunc">trueにしてください</param>
		void AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);

		/// <summary>
		///	入力のサウンドを除外する
		/// </summary>
		/// <param name="sound">除外したいサウンド</param>
		/// <param name="system_value_isCallingAnotherFunc">trueにしてください</param>
		void RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);

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
		void SetEchoParameter(float strength, float delay, float reverb, int effectIndex = -1);

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
		void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex = -1);

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
		/// このノードを削除する
		/// </summary>
		void Destroy(void);

		bool IsActivated(void) { return activated_; }
	private:
		class Node_Impl;
		std::unique_ptr<Node_Impl> impl_;
		friend Node_Impl;

		bool activated_ = false;
	public:

		/// <summary>
		/// <para>生のボイスを取得する</para>
		/// <para>通常は使用する必要なし</para>
		/// </summary>
		/// <returns>サブミックスボイスのポインタ</returns>
		IXAudio2SubmixVoice*& GetVoiceAddress(void);
	};
}
