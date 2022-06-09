#include "../include/SCAL_Effect.h"
#include <xapofx.h>

namespace scal
{
	EffectParameter::~EffectParameter()
	{
		if (type_ == AudioEffectType::VolumeMeter)
		{
			XAUDIO2FX_VOLUMEMETER_LEVELS* level = reinterpret_cast<XAUDIO2FX_VOLUMEMETER_LEVELS*>(param_);
			delete[] level->pPeakLevels;
			delete[] level->pRMSLevels;
			delete param_;

			pEffect_->Release();
		}
	}

	AudioEffectType& EffectParameter::GetEffectType(void)
	{
		return type_;
	}

	IUnknown*& EffectParameter::GetEffectRawAddress(void)
	{
		return pEffect_;
	}

	void*& EffectParameter::GetParameterRawAddress(void)
	{
		return param_;
	}


	void CreateReverb(EffectParameter& param);
	void CreateVolumeMeter(EffectParameter& param, unsigned int channel);
	void CreateEcho(EffectParameter& param);
	void CreateEq(EffectParameter& param);
	void CreateMasteringLimiter(EffectParameter& param);
	void CreateFXReverb(EffectParameter& param);

	void GenerateEffectInstance(EffectParameter& param, AudioEffectType type, unsigned int channel)
	{
		switch (type)
		{
		case AudioEffectType::Reverb:
			CreateReverb(param);
			break;
		case AudioEffectType::VolumeMeter:
			CreateVolumeMeter(param, channel);
			break;
		case AudioEffectType::Echo:
			CreateEcho(param);
			break;
		case AudioEffectType::Equalizer:
			CreateEq(param);
			break;
		case AudioEffectType::MasteringLimiter:
			CreateMasteringLimiter(param);
			break;
		case AudioEffectType::FXReverb:
			CreateFXReverb(param);
			break;
		default:
			break;
		}
	}

	void CreateReverb(EffectParameter& param)
	{
		XAudio2CreateReverb(&param.GetEffectRawAddress());
	}

	void CreateVolumeMeter(EffectParameter& param, unsigned int channel)
	{
		XAudio2CreateVolumeMeter(&param.GetEffectRawAddress());

		XAUDIO2FX_VOLUMEMETER_LEVELS* level = new XAUDIO2FX_VOLUMEMETER_LEVELS();

		level->pPeakLevels = new float[channel];
		level->pRMSLevels = new float[channel];
		level->ChannelCount = channel;
		param.GetParameterRawAddress() = level;
	}

	void CreateEcho(EffectParameter& param)
	{
		CreateFX(__uuidof(FXEcho), &param.GetEffectRawAddress());
	}

	void CreateEq(EffectParameter& param)
	{
		CreateFX(__uuidof(FXEQ), &param.GetEffectRawAddress());
	}

	void CreateMasteringLimiter(EffectParameter& param)
	{
		CreateFX(__uuidof(FXMasteringLimiter), &param.GetEffectRawAddress());
	}

	void CreateFXReverb(EffectParameter& param)
	{
		CreateFX(__uuidof(FXReverb), &param.GetEffectRawAddress());
	}
}

