#pragma once
#include "SCAL_Defines.h"
#include <Windows.h>
#include <xaudio2fx.h>

#pragma comment(lib,"xapobase.lib")

namespace scal
{
	class EffectParameter
	{
	public:
		~EffectParameter();

		AudioEffectType& GetEffectType(void);

		IUnknown*& GetEffectRawAddress(void);
		void*& GetParameterRawAddress(void);
	private:
		AudioEffectType type_;

		IUnknown* pEffect_;
		void* param_;
	};

	void GenerateEffectInstance(EffectParameter& param, AudioEffectType type, unsigned int channel = 0u);
}
