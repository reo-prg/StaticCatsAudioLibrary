#include "../include/SCAL_Node.h"
#include "../include/StaticCatsAudioLibrary.h"
#include "../include/SCAL_Sound.h"
#include "../include/SCAL_Effect.h"
#include <xaudio2.h>
#include <algorithm>
#include <vector>

namespace scal
{
	class Node::Node_Impl
	{
	public:
		void SetVolume(float volume);

		void AddOutputNode(Node* node);
		void AddInputNode(Node* node);
		void RemoveOutputNode(Node* node);
		void RemoveInputNode(Node* node);
		void AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc);
		void RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc);
		bool Create(void);
		void Destroy(void);

		int AddEffect(AudioEffectType type, bool active, int insertPosition);
		bool RemoveEffect(int position);
		bool EnableEffect(int position);
		bool DisableEffect(int position);

		void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex = -1);
		void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex = -1);
		void SetEchoParameter(float strength, float delay, float reverb, int effectIndex = -1);
		void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex = -1);
		void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex = -1);
		void SetFXReverbParameter(float diffuse, float roomsize, int effectIndex = -1);
		XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int effectIndex = -1);

		int FindEffectIndex(AudioEffectType type);

		bool SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping);

		void Terminate(void);

		IXAudio2SubmixVoice*& GetVoiceAddress(void);

		IXAudio2SubmixVoice* submixVoice_ = nullptr;

		std::vector<XAUDIO2_SEND_DESCRIPTOR> send_;

		std::vector<Node*> input_;
		std::vector<Node*> output_;

		std::vector<Sound*> sources_;

		std::vector<XAUDIO2_EFFECT_DESCRIPTOR> efkDesc_;
		std::vector<EffectParameter> efkParam_;

		XAUDIO2_FILTER_PARAMETERS filter_ = {};

		unsigned int stage_ = rootSubmixProcessingStage;
		float volume_ = 1.0f;

		Node* interface_ = nullptr;
	};

	void Node::Node_Impl::Terminate()
	{
		for (auto& i : input_)
		{
			i->RemoveOutputNode(interface_);
		}
		for (auto& o : output_)
		{
			this->RemoveOutputNode(o);
		}
		for (auto& s : sources_)
		{
			this->RemoveInputSound(s, true);
		}

		send_.clear();
		if (submixVoice_ != nullptr)
		{
			submixVoice_->SetOutputVoices(nullptr);

			submixVoice_->DestroyVoice();
		}
	}

	void Node::Node_Impl::SetVolume(float volume)
	{
		submixVoice_->SetVolume(volume);
		volume_ = volume;
	}

	void Node::Node_Impl::AddOutputNode(Node* node)
	{
		output_.push_back(node);
		node->impl_->AddInputNode(interface_);

		stage_ = min(stage_, node->impl_->stage_);

		send_.emplace_back(XAUDIO2_SEND_DESCRIPTOR{ 0, node->GetVoiceAddress()});
	}

	void Node::Node_Impl::AddInputNode(Node* node)
	{
		input_.push_back(node);
	}

	void Node::Node_Impl::RemoveOutputNode(Node* node)
	{
		auto&& it = std::remove_if(output_.begin(), output_.end(), [&node](Node* n) { return node == n; });
		if (it != output_.end())
		{
			(*it)->impl_->RemoveInputNode(interface_);
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
		submixVoice_->SetOutputVoices(&snd);
	}

	void Node::Node_Impl::RemoveInputNode(Node* node)
	{
		auto&& it = std::remove_if(input_.begin(), input_.end(), [&node](Node* n) { return node == n; });
		input_.erase(it, input_.end());
	}

	void Node::Node_Impl::AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc)
	{
		sources_.push_back(sound);
		if (system_value_isCallingAnotherFunc)
		{
			sound->AddOutputNode(interface_);
		}
	}

	void Node::Node_Impl::RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc)
	{
		auto&& it = std::remove_if(sources_.begin(), sources_.end(), [&sound](Sound* s) { return sound == s; });
		if (system_value_isCallingAnotherFunc && it != sources_.end())
		{
			(*it)->RemoveOutputNode(interface_, false);
		}

		sources_.erase(it, sources_.end());
	}

	bool Node::Node_Impl::Create(void)
	{
		auto&& core = GetXAudio2Core();
		auto&& detail = GetMasterDetails();

		HRESULT result;

		result = core->CreateSubmixVoice(&submixVoice_, detail.InputChannels,
			detail.InputSampleRate, XAUDIO2_VOICE_USEFILTER, stage_, nullptr, nullptr);
		if (FAILED(result)) { return false; }

		if (send_.empty())
		{
			submixVoice_->SetOutputVoices(nullptr);
		}
		else
		{
			XAUDIO2_VOICE_SENDS snd = { static_cast<UINT32>(send_.size()), send_.data() };
			submixVoice_->SetOutputVoices(&snd);
		}

		interface_->activated_ = true;

		return true;
	}

	void Node::Node_Impl::Destroy(void)
	{
		for (auto& n : input_)
		{
			n->impl_->RemoveOutputNode(interface_);
		}
		for (auto& n : output_)
		{
			n->impl_->RemoveInputNode(interface_);
		}
		for (auto& n : sources_)
		{
			n->RemoveOutputNode(interface_, false);
		}

		submixVoice_->DestroyVoice();
	}

	int Node::Node_Impl::AddEffect(AudioEffectType type, bool active, int insertPosition)
	{
		EffectParameter param = {};
		auto&& detail = GetMasterDetails();

		GenerateEffectInstance(param, type, detail.InputChannels);

		param.GetEffectType() = type;

		if (insertPosition == -1)
		{
			efkDesc_.emplace_back(XAUDIO2_EFFECT_DESCRIPTOR
				{ param.GetEffectRawAddress(), active, detail.InputChannels});
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

		submixVoice_->SetEffectChain(nullptr);
		submixVoice_->SetEffectChain(&chain);

		return insertPosition;
	}

	bool Node::Node_Impl::RemoveEffect(int position)
	{
		HRESULT result;

		if (position == -1)
		{
			efkDesc_.clear();
			efkParam_.clear();
			result = submixVoice_->SetEffectChain(nullptr);
			return SUCCEEDED(result);
		}

		position = min(position, static_cast<int>(efkDesc_.size()) - 1);

		efkDesc_.erase(efkDesc_.begin() + position);
		efkParam_.erase(efkParam_.begin() + position);

		XAUDIO2_EFFECT_CHAIN chain = { static_cast<UINT32>(efkDesc_.size()), efkDesc_.data() };

		result = submixVoice_->SetEffectChain(nullptr);
		if (FAILED(result)) { return false; }

		if (efkDesc_.size() != 0)
		{
			result = submixVoice_->SetEffectChain(&chain);
		}
		return SUCCEEDED(result);
	}

	bool Node::Node_Impl::EnableEffect(int position)
	{
		HRESULT result = S_OK;

		if (position == -1)
		{
			for (int i = 0; i < efkDesc_.size(); i++)
			{
				result = submixVoice_->EnableEffect(i);
				if (FAILED(result)) { break; }
			}
		}
		else
		{
			position = min(position, static_cast<int>(efkDesc_.size() - 1));
			result = submixVoice_->EnableEffect(position);
		}
		return SUCCEEDED(result);
	}

	bool Node::Node_Impl::DisableEffect(int position)
	{
		HRESULT result = S_OK;

		if (position == -1)
		{
			for (int i = 0; i < efkDesc_.size(); i++)
			{
				result = submixVoice_->DisableEffect(i);
				if (FAILED(result)) { break; }
			}
		}
		else
		{
			position = min(position, static_cast<int>(efkDesc_.size()) - 1);
			result = submixVoice_->DisableEffect(position);
		}
		return SUCCEEDED(result);
	}

	void Node::Node_Impl::SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex)
	{
		XAUDIO2FX_REVERB_PARAMETERS p = {};
		ReverbConvertI3DL2ToNative(&param, &p);
		SetReverbParameter(p, effectIndex);
	}

	void Node::Node_Impl::SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Reverb);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		result = submixVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Node::Node_Impl::SetEchoParameter(float strength, float delay, float reverb, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Echo);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXECHO_PARAMETERS param = { strength, delay, reverb };
		result = submixVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Node::Node_Impl::SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::Equalizer);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;
		result = submixVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Node::Node_Impl::SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::MasteringLimiter);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXMASTERINGLIMITER_PARAMETERS param = { release, loudness };
		result = submixVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result)) { OutputDebugStringA("SetEffectParameter is failed\n"); }
	}

	void Node::Node_Impl::SetFXReverbParameter(float diffuse, float roomsize, int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::FXReverb);
			if (effectIndex < 0) { return; }
		}

		HRESULT result;

		FXREVERB_PARAMETERS param = { diffuse, roomsize };
		result = submixVoice_->SetEffectParameters(effectIndex, &param, sizeof(param));
		if (FAILED(result))
		{
			OutputDebugStringA("SetEffectParameter is failed\n");
		}
	}

	XAUDIO2FX_VOLUMEMETER_LEVELS* Node::Node_Impl::GetVolumeMeterParameter(int effectIndex)
	{
		if (effectIndex < 0)
		{
			effectIndex = FindEffectIndex(AudioEffectType::VolumeMeter);
			if (effectIndex < 0) { return nullptr; }
		}

		auto&& efkAddress = efkParam_[effectIndex].GetParameterRawAddress();
		submixVoice_->GetEffectParameters(effectIndex, efkAddress, sizeof(XAUDIO2FX_VOLUMEMETER_LEVELS));
		return reinterpret_cast<XAUDIO2FX_VOLUMEMETER_LEVELS*>(efkAddress);
	}

	int Node::Node_Impl::FindEffectIndex(AudioEffectType type)
	{
		int ret;
		for (ret = static_cast<int>(efkParam_.size()) - 1; ret >= 0; ret--)
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

	bool Node::Node_Impl::SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping)
	{
		frequency = std::clamp(frequency, 0.0f, XAUDIO2_MAX_FILTER_FREQUENCY);
		danping = std::clamp(danping, 0.0f, XAUDIO2_MAX_FILTER_ONEOVERQ);
		filter_ = { type, frequency, danping };

		HRESULT result;
		
		result = submixVoice_->SetFilterParameters(&filter_);

		return SUCCEEDED(result);
	}

	IXAudio2SubmixVoice*& Node::Node_Impl::GetVoiceAddress(void)
	{
		return submixVoice_;
	}


	scal::Node::Node()
	{
		impl_ = std::make_unique<Node_Impl>();
		impl_->interface_ = this;
	}

	scal::Node::~Node()
	{
		impl_->Terminate();
		impl_.reset();
	}

	void Node::SetVolume(float volume)
	{
		impl_->SetVolume(volume);
	}

	void Node::AddOutputNode(Node* node)
	{
		impl_->AddOutputNode(node);
	}

	void Node::RemoveOutputNode(Node * node)
	{
		impl_->RemoveOutputNode(node);
	}
	void Node::AddInputSound(Sound* sound, bool system_value_isCallingAnotherFunc)
	{
		impl_->AddInputSound(sound, system_value_isCallingAnotherFunc);
	}
	void Node::RemoveInputSound(Sound* sound, bool system_value_isCallingAnotherFunc)
	{
		impl_->RemoveInputSound(sound, system_value_isCallingAnotherFunc);
	}
	int Node::AddEffect(AudioEffectType type, bool active, int insertPosition)
	{
		return impl_->AddEffect(type, active, insertPosition);
	}
	bool Node::RemoveEffect(int position)
	{
		return impl_->RemoveEffect(position);
	}
	bool Node::EnableEffect(int position)
	{
		return impl_->EnableEffect(position);
	}
	bool Node::DisableEffect(int position)
	{
		return impl_->DisableEffect(position);
	}
	void Node::SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int effectIndex)
	{
		impl_->SetReverbParameter(param, effectIndex);
	}
	void Node::SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int effectIndex)
	{
		impl_->SetReverbParameter(param, effectIndex);
	}
	void Node::SetEchoParameter(float strength, float delay, float reverb, int effectIndex)
	{
		impl_->SetEchoParameter(strength, delay, reverb, effectIndex);
	}
	void Node::SetEqualizerParameter(const FXEQ_PARAMETERS& param, int effectIndex)
	{
		impl_->SetEqualizerParameter(param, effectIndex);
	}
	void Node::SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int effectIndex)
	{
		impl_->SetMasteringLimiterParameter(release, loudness, effectIndex);
	}
	void Node::SetFXReverbParameter(float diffuse, float roomsize, int effectIndex)
	{
		impl_->SetFXReverbParameter(diffuse, roomsize, effectIndex);
	}
	XAUDIO2FX_VOLUMEMETER_LEVELS* Node::GetVolumeMeterParameter(int effectIndex)
	{
		return impl_->GetVolumeMeterParameter(effectIndex);
	}
	int Node::FindEffectIndex(AudioEffectType type)
	{
		return impl_->FindEffectIndex(type);
	}
	bool Node::SetFilter(XAUDIO2_FILTER_TYPE type, float frequency, float danping)
	{
		return impl_->SetFilter(type, frequency, danping);
	}
	bool Node::Create(void)
	{
		return impl_->Create();
	}
	void Node::Destroy(void)
	{
		impl_->Destroy();
	}
	IXAudio2SubmixVoice*& Node::GetVoiceAddress(void)
	{
		return impl_->GetVoiceAddress();
	}
}
