#pragma once
#include "SCAL_Defines.h"
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <memory>
#include <string>

namespace scal
{
	class Node;

	class Sound
	{
	public:
		Sound();
		Sound(const std::string& filepath);
		virtual ~Sound();
		
		bool Load(const std::string& filepath);

		bool Play(void);
		
		void SetPlaySegment(float begin = 0.0f, float length = 0.0f);
		void SetLoopSegment(float begin = 0.0f, float length = 0.0f);
		void SetLoopCount(UINT32 count = XAUDIO2_LOOP_INFINITE);
		void SetVolume(float volume = 1.0f);


		void AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);
		void RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc = true);

		int AddEffect(AudioEffectType type, bool active, int insertPosition = -1);
		bool RemoveEffect(int position = -1);

		bool EnableEffect(int position = -1);
		bool DisableEffect(int position = -1);

		void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex = -1);
		void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex = -1);
		void SetEchoParameter(float strength, float delay, float reverb, int effectIndex = -1);
		void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex = -1);
		void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex = -1);
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);

		int FindEffectIndex(AudioEffectType type);


		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		float GetProgress(void);

		bool Stop(void);
		void Destroy(void);

		IXAudio2SourceVoice*& GetVoiceAddress(void);

		bool IsActivated(void) { return activated_; }

	protected:
		class Sound_Impl;
		std::unique_ptr<Sound_Impl> impl_;

		bool activated_ = false;
	};


	class SoundEmitter
		: public Sound
	{
	public:
		SoundEmitter();
		SoundEmitter(const std::string& filepath);
		~SoundEmitter();


	private:
		
	};

}
