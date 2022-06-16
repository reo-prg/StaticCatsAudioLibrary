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

	class Node
	{
	public:
		Node();
		~Node();

		void SetVolume(float volume);

		void AddOutputNode(Node* node);
		void RemoveOutputNode(Node* node);


		void AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);
		void RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc = true);


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

		bool Create(void);

		void Destroy(void);

		IXAudio2SubmixVoice*& GetVoiceAddress(void);

		bool IsActivated(void) { return activated_; }
	private:
		class Node_Impl;
		std::unique_ptr<Node_Impl> impl_;
		friend Node_Impl;

		bool activated_ = false;
	};
}
