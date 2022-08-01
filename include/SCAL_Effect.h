#pragma once
#include <Windows.h>
#include "SCAL_Defines.h"
#include <xaudio2fx.h>

//#pragma comment(lib,"xapobase.lib")

namespace scal
{
	/// <summary>
	/// <para>サウンドエフェクトのクラス</para>
	/// <para>SoundやNodeが自分で管理しているので</para>
	/// <para>使用する必要なし</para>
	/// </summary>
	class EffectParameter
	{
	public:
		~EffectParameter();

		/// <summary>
		/// エフェクトの種類を取得する
		/// </summary>
		/// <returns>エフェクトの種類</returns>
		AudioEffectType& GetEffectType(void);

		/// <summary>
		/// 生のエフェクトのアドレスを取得する
		/// </summary>
		/// <returns>エフェクトのアドレス</returns>
		IUnknown*& GetEffectRawAddress(void);

		/// <summary>
		/// エフェクトのパラメーターを取得する
		/// </summary>
		/// <returns>パラメーター</returns>
		void*& GetParameterRawAddress(void);
	private:
		AudioEffectType type_;

		IUnknown* pEffect_;
		void* param_;
	};

	/// <summary>
	/// <para>エフェクトを生成する</para>
	/// <para>SoundやNodeが自分で呼び出すので</para>
	/// <para>使用する必要なし</para>
	/// </summary>
	/// <param name="param">サウンドエフェクト</param>
	/// <param name="type">エフェクトの種類</param>
	/// <param name="channel">チャンネル数</param>
	void GenerateEffectInstance(EffectParameter& param, AudioEffectType type, unsigned int channel = 0u);
}
