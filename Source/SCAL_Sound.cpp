#include "../include/SCAL_Sound.h"
#include "../include/StaticCatsAudioLibrary.h"
#include "../include/SCAL_Effect.h"
#include "../include/SCAL_Node.h"
#include "../include/SCAL_Math.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <algorithm>
#include <vector>

using namespace DirectX;

namespace scal
{
	using namespace xm_operator;

	class Sound::Sound_Impl
	{
	public:
		Sound_Impl(){};
		Sound_Impl(const std::string& filepath);

		// Sound ----------------------------------------------------------
		bool Load(const std::string& filepath);
		bool Play(void);
		void SetPlaySegment(float begin, float length);
		void SetLoopSegment(float begin, float length);
		void SetLoopCount(UINT32 count = -1);
		void SetVolume(float volume);
		void AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc);
		void RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc);
		bool Stop(void);
		void Destroy(void);

		float GetProgress(void);

		int AddEffect(AudioEffectType type, bool active, int insertPosition);
		bool RemoveEffect(int position);

		bool EnableEffect(int position) ;
		bool DisableEffect(int position);

		void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex = -1);
		void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex = -1);
		void SetEchoParameter(float feedbackVolume, float delay, float effectStrength, int effectIndex = -1);
		void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex = -1);
		void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex = -1);
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);

		int FindEffectIndex(AudioEffectType type);

		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		void Terminate(void);

		IXAudio2SourceVoice*& GetVoiceAddress(void);


		WAVEFORMATEX waveFormat_;
		XAUDIO2_BUFFER buffer_;
		IXAudio2SourceVoice* sourceVoice_ = nullptr;
		SoundState vState_;

		float volume_ = 1.0f;

		std::vector<XAUDIO2_SEND_DESCRIPTOR> send_;
		std::vector<Node*> output_;

		std::vector<XAUDIO2_EFFECT_DESCRIPTOR> efkDesc_;
		std::vector<EffectParameter> efkParam_;

		XAUDIO2_FILTER_PARAMETERS filter_;

		Sound* interface_;
	};

	Sound::Sound_Impl::Sound_Impl(const std::string& filepath)
	{
		Load(filepath);
	}

	void Sound::Sound_Impl::Terminate()
	{
		for (auto& o : output_)
		{
			o->RemoveInputSound(interface_, false);
		}

		send_.clear();
		XAUDIO2_VOICE_SENDS snd = { static_cast<UINT32>(send_.size()), send_.data() };
		sourceVoice_->SetOutputVoices(&snd);

		if (sourceVoice_ != nullptr)
		{
			sourceVoice_->DestroyVoice();
		}
	}

	bool Sound::Sound_Impl::Load(const std::string& filepath)
	{
		Destroy();

		auto&& data = GetSoundData(filepath);

		HRESULT result;

		waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat_.nChannels = data.fmt_.channel_;
		waveFormat_.nSamplesPerSec = data.fmt_.samplesPerSec_;
		waveFormat_.nAvgBytesPerSec = data.fmt_.bytePerSec_;
		waveFormat_.nBlockAlign = data.fmt_.blockAlign_;
		waveFormat_.wBitsPerSample = data.fmt_.bitPerSample_;

		buffer_.AudioBytes = data.dataSize_;
		buffer_.pAudioData = data.data_;
		buffer_.PlayBegin = 0;
		buffer_.PlayLength = 0;
		buffer_.LoopBegin = 0;
		buffer_.LoopCount = 0;
		buffer_.LoopLength = 0;
		buffer_.Flags = XAUDIO2_END_OF_STREAM;

		auto&& core = GetXAudio2Core();

		result = core->CreateSourceVoice(&sourceVoice_, &waveFormat_,
			XAUDIO2_VOICE_USEFILTER, 4.0f);
		if (FAILED(result)) { return false; }

		return true;
	}

	bool Sound::Sound_Impl::Play(void)
	{
		HRESULT result;

		result = sourceVoice_->SubmitSourceBuffer(&buffer_);
		if (FAILED(result)) { return false; }

		vState_ = SoundState::Playing;
		result = sourceVoice_->Start();
		if (FAILED(result)) { return false; }

		return true;
	}

	void Sound::Sound_Impl::SetPlaySegment(float begin, float length)
	{
		buffer_.PlayBegin = static_cast<UINT32>(waveFormat_.nSamplesPerSec * begin);
		buffer_.PlayLength = static_cast<UINT32>(waveFormat_.nSamplesPerSec * length);
	}

	void Sound::Sound_Impl::SetLoopSegment(float begin, float length)
	{
		buffer_.LoopBegin = static_cast<UINT32>(waveFormat_.nSamplesPerSec * begin);
		buffer_.LoopLength = static_cast<UINT32>(waveFormat_.nSamplesPerSec * length);
	}

	void Sound::Sound_Impl::SetLoopCount(UINT32 count)
	{
		buffer_.LoopCount = count;
	}

	void Sound::Sound_Impl::SetVolume(float volume)
	{
		volume_ = volume;
		sourceVoice_->SetVolume(volume);
	}

	void Sound::Sound_Impl::AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc)
	{
		output_.push_back(node);
		if (system_value_isCallingAnotherFunc)
		{
			node->AddInputSound(interface_, false);
		}

		send_.emplace_back(XAUDIO2_SEND_DESCRIPTOR{ 0, node->GetVoiceAddress() });

		XAUDIO2_VOICE_SENDS snd = { static_cast<UINT32>(send_.size()), send_.data() };
		sourceVoice_->SetOutputVoices(&snd);
	}

	void Sound::Sound_Impl::RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc)
	{
		auto&& it = std::remove_if(output_.begin(), output_.end(), [&node](Node* n) { return node == n; });

		if (system_value_isCallingAnotherFunc && it != output_.end())
		{
			node->RemoveInputSound(interface_, false);
		}
		output_.erase(it, output_.end());

		auto&& it2 = std::remove_if(send_.begin(), send_.end(),
			[&node](const XAUDIO2_SEND_DESCRIPTOR& n) { return n.pOutputVoice == node->GetVoiceAddress(); });
		send_.erase(it2, send_.end());

		XAUDIO2_VOICE_SENDS snd = { static_cast<UINT32>(send_.size()), send_.data() };
		if (snd.SendCount == 0)
		{
			snd.pSends = nullptr;
		}
		sourceVoice_->SetOutputVoices(&snd);
	}

	bool Sound::Sound_Impl::Stop(void)
	{
		if (vState_ == SoundState::Stop) { return true; }

		HRESULT result;
		result = sourceVoice_->Stop();
		if (FAILED(result)) { return false; }
		
		vState_ = SoundState::Stop;
		return true;
	}

	void Sound::Sound_Impl::Destroy(void)
	{
		if (sourceVoice_ == nullptr) { return; }

		for (auto& n : output_)
		{
			n->RemoveInputSound(interface_, false);
		}

		interface_->activated_ = false;
		sourceVoice_->DestroyVoice();
		sourceVoice_ = nullptr;
		vState_ = SoundState::Stop;
	}

	float Sound::Sound_Impl::GetProgress(void)
	{
		XAUDIO2_VOICE_STATE sys_state;
		sourceVoice_->GetState(&sys_state, 0);

		return (static_cast<float>(sys_state.SamplesPlayed) / static_cast<float>(waveFormat_.nSamplesPerSec))
			/ (static_cast<float>(buffer_.AudioBytes) / static_cast<float>(waveFormat_.nAvgBytesPerSec));
	}

	int Sound::Sound_Impl::AddEffect(AudioEffectType type, bool active, int insertPosition)
	{
		EffectParameter param = {};
		auto&& detail = GetMasterDetails();

		GenerateEffectInstance(param, type, detail.InputChannels);

		param.GetEffectType() = type;

		if (insertPosition == -1)
		{
			efkDesc_.emplace_back(XAUDIO2_EFFECT_DESCRIPTOR
				{ param.GetEffectRawAddress(), active, detail.InputChannels });
			efkParam_.emplace_back(param);
			insertPosition = static_cast<int>(efkDesc_.size()) - 1;
		}
		else
		{
			insertPosition = min(insertPosition, static_cast<int>(efkDesc_.size()));

			efkDesc_.emplace(efkDesc_.begin() + insertPosition,
				XAUDIO2_EFFECT_DESCRIPTOR{ param.GetEffectRawAddress(), active,
				detail.InputChannels });
			efkParam_.emplace(efkParam_.begin(), param);
		}
		XAUDIO2_EFFECT_CHAIN chain = { static_cast<UINT32>(efkDesc_.size()), efkDesc_.data() };

		sourceVoice_->SetEffectChain(nullptr);
		sourceVoice_->SetEffectChain(&chain);

		return insertPosition;
	}

	bool Sound::Sound_Impl::RemoveEffect(int position)
	{
		HRESULT result;

		if (position == -1)
		{
			efkDesc_.clear();
			efkParam_.clear();
			result = sourceVoice_->SetEffectChain(nullptr);
			return SUCCEEDED(result);
		}

		position = min(position, static_cast<int>(efkDesc_.size()) - 1);

		efkDesc_.erase(efkDesc_.begin() + position);
		efkParam_.erase(efkParam_.begin() + position);

		XAUDIO2_EFFECT_CHAIN chain = { static_cast<UINT32>(efkDesc_.size()), efkDesc_.data() };

		result = sourceVoice_->SetEffectChain(nullptr);
		if (FAILED(result)) { return false; }

		if (efkDesc_.size() != 0)
		{
			result = sourceVoice_->SetEffectChain(&chain);
		}
		return SUCCEEDED(result);
	}

	bool Sound::Sound_Impl::EnableEffect(int position)
	{
		HRESULT result = S_OK;

		if (position == -1)
		{
			for (int i = 0; i < efkDesc_.size(); i++)
			{
				result = sourceVoice_->EnableEffect(i);
				if (FAILED(result)) { break; }
			}
		}
		else
		{
			position = min(position, static_cast<int>(efkDesc_.size()) - 1);
			result = sourceVoice_->EnableEffect(position);
		}

		return SUCCEEDED(result);
	}

	bool Sound::Sound_Impl::DisableEffect(int position)
	{
		HRESULT result = S_OK;

		if (position == -1)
		{
			for (int i = 0; i < efkDesc_.size(); i++)
			{
				result = sourceVoice_->DisableEffect(i);
				if (FAILED(result)) { break; }
			}
		}
		else
		{
			position = min(position, static_cast<int>(efkDesc_.size()) - 1);
			result = sourceVoice_->DisableEffect(position);
		}
		return SUCCEEDED(result);
	}

	bool Sound::Sound_Impl::SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping)
	{
		HRESULT result;

		frequency = std::clamp(frequency, 0.0f, XAUDIO2_MAX_FILTER_FREQUENCY);
		danping = std::clamp(danping, 0.0f, XAUDIO2_MAX_FILTER_ONEOVERQ);
		filter_ = { type, frequency, danping };

		result = sourceVoice_->SetFilterParameters(&filter_);
		return SUCCEEDED(result);
	}

	IXAudio2SourceVoice*& Sound::Sound_Impl::GetVoiceAddress(void)
	{
		return sourceVoice_;
	}

	void Sound::Sound_Impl::SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex)
	{
		XAUDIO2FX_REVERB_PARAMETERS p = {};
		ReverbConvertI3DL2ToNative(&param, &p);
		SetReverbParameter(p, effectIndex);
	}

	void Sound::Sound_Impl::SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Reverb);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		result = sourceVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Sound::Sound_Impl::SetEchoParameter(float feedbackVolume, float delay, float effectStrength, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Echo);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXECHO_PARAMETERS param = { effectStrength, feedbackVolume, delay };
		result = sourceVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Sound::Sound_Impl::SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Equalizer);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;
		result = sourceVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Sound::Sound_Impl::SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::MasteringLimiter);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXMASTERINGLIMITER_PARAMETERS param = { release, loudness };
		result = sourceVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Sound::Sound_Impl::SetFXReverbParameter(float diffuse, float roomsize, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::FXReverb);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXREVERB_PARAMETERS param = { diffuse, roomsize };
		result = sourceVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result))
		{
			OutputDebugStringA("SetEffectParameter is failed\n");
		}
	}

	XAUDIO2FX_VOLUMEMETER_LEVELS* Sound::Sound_Impl::GetVolumeMeterParameter(int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::VolumeMeter);
			if (effectIndex < 0) { return nullptr; }
		}

		auto&& efkAddress = efkParam_[effectIndex].GetParameterRawAddress();
		sourceVoice_->GetEffectParameters(effectIndex, efkAddress, sizeof(XAUDIO2FX_VOLUMEMETER_LEVELS));
		return reinterpret_cast<XAUDIO2FX_VOLUMEMETER_LEVELS*>(efkAddress);
	}

	int Sound::Sound_Impl::FindEffectIndex(AudioEffectType type)
	{
		int ret;
		for (ret = static_cast<int>(efkParam_.size() - 1); ret >= 0; ret--)
		{
			if (efkParam_[ret].GetEffectType() == type)
			{
				break;
			}
		}

		if (ret < 0)
		{
			ret = -1;
		}

		return ret;
	}



	Sound::Sound()
	{
		impl_ = std::make_unique<Sound_Impl>();
		impl_->interface_ = this;
	}

	Sound::Sound(const std::string& filepath)
	{
		impl_ = std::make_unique<Sound_Impl>(filepath);
		impl_->interface_ = this;
	}

	Sound::~Sound()
	{
		impl_->Terminate();
		impl_.reset();
	}

	bool Sound::Load(const std::string & filepath)
	{
		if (activated_)
		{
			impl_->Destroy();
		}

		activated_ = impl_->Load(filepath);
		return activated_;
	}

	bool Sound::Play(void)
	{
		if (!activated_)
		{
			OutputDebugStringA("SCAL_ERROR : Sound is not loaded\n");
			return false;
		}

		return impl_->Play();
	}

	void Sound::SetPlaySegment(float begin, float length)
	{
		impl_->SetPlaySegment(begin, length);
	}

	void Sound::SetLoopSegment(float begin, float length)
	{
		impl_->SetLoopSegment(begin, length);
	}

	void Sound::SetLoopCount(UINT32 count)
	{
		impl_->SetLoopCount(count);
	}

	void Sound::SetVolume(float volume)
	{
		impl_->SetVolume(volume);
	}

	void Sound::AddOutputNode(Node* node, bool system_value_isCallingAnotherFunc)
	{
		impl_->AddOutputNode(node, system_value_isCallingAnotherFunc);
	}

	void Sound::RemoveOutputNode(Node* node, bool system_value_isCallingAnotherFunc)
	{
		impl_->RemoveOutputNode(node, system_value_isCallingAnotherFunc);
	}

	int Sound::AddEffect(AudioEffectType type, bool active, int insertPosition)
	{
		return impl_->AddEffect(type, active, insertPosition);
	}

	bool Sound::RemoveEffect(int position)
	{
		return impl_->RemoveEffect(position);
	}

	bool Sound::EnableEffect(int position)
	{
		return impl_->EnableEffect(position);
	}

	bool Sound::DisableEffect(int position)
	{
		return impl_->DisableEffect(position);
	}
	void Sound::SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex)
	{
		impl_->SetReverbParameter(param, effectIndex);
	}
	void Sound::SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex)
	{
		impl_->SetReverbParameter(param, effectIndex);
	}
	void Sound::SetEchoParameter(float feedbackVolume, float delay, float effectStrength, int effectIndex)
	{
		impl_->SetEchoParameter(feedbackVolume, delay, effectStrength, effectIndex);
	}
	void Sound::SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex)
	{
		impl_->SetEqualizerParameter(param, effectIndex);
	}
	void Sound::SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex)
	{
		impl_->SetMasteringLimiterParameter(release, loudness, effectIndex);
	}
	void Sound::SetFXReverbParameter(float diffuse, float roomsize, int effectIndex)
	{
		impl_->SetFXReverbParameter(diffuse, roomsize, effectIndex);
	}
	XAUDIO2FX_VOLUMEMETER_LEVELS* Sound::GetVolumeMeterParameter(int effectIndex)
	{
		return impl_->GetVolumeMeterParameter(effectIndex);
	}
	int Sound::FindEffectIndex(AudioEffectType type)
	{
		return impl_->FindEffectIndex(type);
	}
	bool Sound::SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping)
	{
		return impl_->SetFilter(type, frequency, danping);
	}

	float Sound::GetProgress(void)
	{
		return impl_->GetProgress();
	}

	bool Sound::Stop(void)
	{
		return impl_->Stop();
	}

	void Sound::Destroy(void)
	{
		impl_->Destroy();
	}

	IXAudio2SourceVoice*& Sound::GetVoiceAddress(void)
	{
		return impl_->GetVoiceAddress();
	}

	void Sound::GetSoundInnerData(SoundEmitter* emitter)
	{
		emitter->SetSoundInnerData(impl_.get());
	}


	// Emitter---------------------------------------------------------------------------------

	class SoundEmitter::Emitter_Impl
	{
	public:
		Emitter_Impl() = default;
		Emitter_Impl(Sound* sound);
		~Emitter_Impl();

		void SetSound(Sound* sound);

		void SetPosition(const Vector3& pos);

		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		void SetRotate(const Vector3& axis, float rot);
		void SetRotate(float x, float y, float z);
		void SetRotateAsQuaternion(const Quaternion& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const Quaternion& quat);

		Quaternion& GetRotateQuaternionRef(void);

		void EnableVelocity(bool enable = true);

		void UpdateVelocity(void);

		void SetSoundInnerData(Sound::Sound_Impl* data);

		Sound* sound_ = nullptr;
		Sound::Sound_Impl* soundData_ = nullptr;
		SoundEmitter* interface_;

		X3DAUDIO_EMITTER emitter_;
		X3DAUDIO_DSP_SETTINGS dsp_;

		Vector3 defaultFrontVector_;
		Vector3 defaultUpVector_;

		Vector3 lastpos_;

		Quaternion rotate_;

		bool isVelocityEnable_ = false;
	private:
		void Apply(void);

		bool isPositionUpdated_ = false;

		void Initialize(void);
	};

	SoundEmitter::Emitter_Impl::Emitter_Impl(Sound* sound)
		:sound_(sound)
	{
		Initialize();
	}

	SoundEmitter::Emitter_Impl::~Emitter_Impl()
	{
	}

	void SoundEmitter::Emitter_Impl::SetSound(Sound* sound)
	{
		sound_ = sound;
		Initialize();
	}

	void SoundEmitter::Emitter_Impl::SetPosition(const Vector3& pos)
	{
		emitter_.Position = { pos.x, pos.y, pos.z };
	}

	void SoundEmitter::Emitter_Impl::SetDefaultDirection(const Vector3& front, const Vector3& up)
	{
		defaultFrontVector_ = front;
		defaultUpVector_ = up;

		Apply();
	}

	void SoundEmitter::Emitter_Impl::SetRotate(const Vector3& axis, float rot)
	{
		rotate_.SetRotationAxis(axis, rot);

		Apply();
	}

	void SoundEmitter::Emitter_Impl::SetRotate(float x, float y, float z)
	{
		rotate_.SetRotationEulerAngle(x, y, z);

		Apply();
	}

	void SoundEmitter::Emitter_Impl::SetRotateAsQuaternion(const Quaternion& quat)
	{
		rotate_ = quat;
		Apply();
	}

	void SoundEmitter::Emitter_Impl::AddRotate(const Vector3& axis, float rot)
	{
		auto&& q = Quaternion::RotateAxis(axis, rot);

		rotate_ = rotate_ * q;

		Apply();
	}

	void SoundEmitter::Emitter_Impl::AddRotateAsQuaternion(const Quaternion& quat)
	{
		rotate_ = rotate_ * quat;
		Apply();
	}

	Quaternion& SoundEmitter::Emitter_Impl::GetRotateQuaternionRef(void)
	{
		return rotate_;
	}

	void SoundEmitter::Emitter_Impl::EnableVelocity(bool enable)
	{
		isVelocityEnable_ = enable;
	}

	void SoundEmitter::Emitter_Impl::UpdateVelocity(void)
	{
		if (!isVelocityEnable_) { return; }

		emitter_.Velocity = GetFloat3(emitter_.Position - lastpos_);

		lastpos_ = emitter_.Position;
	}

	void SoundEmitter::Emitter_Impl::SetSoundInnerData(Sound::Sound_Impl* data)
	{
		soundData_ = data;
	}

	void SoundEmitter::Emitter_Impl::Apply(void)
	{
		auto&& m = rotate_.GetRotationMatrix();

		auto&& f = TranslationMatrix(defaultFrontVector_);
		auto&& u = TranslationMatrix(defaultUpVector_);


		emitter_.OrientFront = { f.matrix_[0][3], f.matrix_[1][3], f.matrix_[2][3] };
		emitter_.OrientTop = { u.matrix_[0][3], u.matrix_[1][3], u.matrix_[2][3] };
	}

	void SoundEmitter::Emitter_Impl::Initialize(void)
	{
		sound_->GetSoundInnerData(interface_);

		emitter_ = { 0 };

		auto&& d = GetMasterDetails();
		emitter_.ChannelCount = soundData_->waveFormat_.nChannels;
		emitter_.CurveDistanceScaler = FLT_MIN;
	}


	SoundEmitter::SoundEmitter()
	{
		impl_ = std::make_unique<Emitter_Impl>();
		impl_->interface_ = this;
	}

	SoundEmitter::SoundEmitter(Sound* sound)
	{
		impl_ = std::make_unique<Emitter_Impl>(sound);
		impl_->interface_ = this;
	}

	SoundEmitter::~SoundEmitter()
	{
	}

	void SoundEmitter::SetSound(Sound* sound)
	{
		impl_->SetSound(sound);
		Initialize();
	}

	void SoundEmitter::SetPosition(const Vector3& pos)
	{
		impl_->SetPosition(pos);
	}

	void SoundEmitter::SetDefaultDirection(const Vector3& front, const Vector3& up)
	{
		impl_->SetDefaultDirection(front, up);
	}

	void SoundEmitter::SetRotate(const Vector3& axis, float rot)
	{
		impl_->SetRotate(axis, rot);
	}

	void SoundEmitter::SetRotate(float x, float y, float z)
	{
		impl_->SetRotate(x, y, z);
	}

	void SoundEmitter::SetRotateAsQuaternion(const Quaternion& quat)
	{
		impl_->SetRotateAsQuaternion(quat);
	}

	void SoundEmitter::AddRotate(const Vector3& axis, float rot)
	{
		impl_->AddRotate(axis, rot);
	}

	void SoundEmitter::AddRotateAsQuaternion(const Quaternion& quat)
	{
		impl_->AddRotateAsQuaternion(quat);
	}

	Quaternion& SoundEmitter::GetRotateQuaternionRef(void)
	{
		return impl_->GetRotateQuaternionRef();
	}

	void SoundEmitter::EnableVelocity(bool enable)
	{
		impl_->EnableVelocity(enable);
	}

	void SoundEmitter::UpdateVelocity(void)
	{
		impl_->UpdateVelocity();
	}

	void SoundEmitter::SetSoundInnerData(Sound::Sound_Impl* data)
	{
		impl_->SetSoundInnerData(data);
	}
}