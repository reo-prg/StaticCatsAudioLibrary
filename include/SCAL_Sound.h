#pragma once
#include "SCAL_Defines.h"
#include <DirectXMath.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <memory>
#include <string>
#include "SCAL_Math.h"

namespace scal
{
	class Node;
	class SoundEmitter;

	/// <summary>
	/// Sound�N���X
	/// �����Đ�������ł���N���X�ł�
	/// </summary>
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

		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		void SetRotate(const Vector3& axis, float rot);
		void SetRotate(float x, float y, float z);
		void SetRotateAsQuaternion(const DirectX::XMVECTOR& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const DirectX::XMVECTOR& quat);
		void EnableVelocity(bool enable = true);

		void UpdateVelocity(void);



		void SetSoundInnerData(Sound::Sound_Impl* data);
	private:
		class Emitter_Impl;
		std::unique_ptr<Emitter_Impl> impl_;
	};

}
