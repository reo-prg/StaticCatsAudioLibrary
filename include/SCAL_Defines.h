#pragma once

namespace scal
{
	/// <summary>
	/// サウンドエフェクトの種類
	/// </summary>
	enum class AudioEffectType
	{
		Reverb,				// リバーブ
		VolumeMeter,		// ボリュームメーター(音量を取得できる)
		Echo,				// エコー
		Equalizer,			// イコライザー
		MasteringLimiter,	// マスタリングリミッター
		FXReverb,			// リバーブ(xapofx)
	};
}
