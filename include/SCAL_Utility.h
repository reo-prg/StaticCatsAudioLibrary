#pragma once
#include <string>
#include <Windows.h>

namespace scal_util
{
	/// <summary>
	/// マルチバイト文字をワイド文字に変換する
	/// </summary>
	/// <param name="str">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	std::wstring StringToWString(const std::string& str)
	{
		std::wstring ret;
		int val1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
		ret.resize(val1);
		int val2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &ret[0], val1);
		return ret;
	}

	/// <summary>
	/// 拡張子を取得する
	/// </summary>
	/// <param name="str">ファイルパス</param>
	/// <returns>拡張子("sound.wav" なら "wav")</returns>
	std::string GetExtension(const std::string& str)
	{
		size_t idx = str.rfind(".");
		if (idx == std::string::npos)
		{
			return std::string();
		}

		return str.substr(idx + 1);
	}

	/// <summary>
	/// 線形補間をする
	/// </summary>
	/// <param name="val1">1つ目の値</param>
	/// <param name="val2">2つ目の値</param>
	/// <param name="t">補間値</param>
	/// <returns>val1 + (val2 - val1) * t</returns>
	float Lerp(float val1, float val2, float t)
	{
		return val1 + (val2 - val1) * t;
	}
}
