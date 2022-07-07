#pragma once
#include <string>
#include <Windows.h>

namespace scal_util
{
	/// <summary>
	/// �}���`�o�C�g���������C�h�����ɕϊ�����
	/// </summary>
	/// <param name="str">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	std::wstring StringToWString(const std::string& str)
	{
		std::wstring ret;
		int val1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
		ret.resize(val1);
		int val2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &ret[0], val1);
		return ret;
	}

	/// <summary>
	/// �g���q���擾����
	/// </summary>
	/// <param name="str">�t�@�C���p�X</param>
	/// <returns>�g���q("sound.wav" �Ȃ� "wav")</returns>
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
	/// ���`��Ԃ�����
	/// </summary>
	/// <param name="val1">1�ڂ̒l</param>
	/// <param name="val2">2�ڂ̒l</param>
	/// <param name="t">��Ԓl</param>
	/// <returns>val1 + (val2 - val1) * t</returns>
	float Lerp(float val1, float val2, float t)
	{
		return val1 + (val2 - val1) * t;
	}
}
