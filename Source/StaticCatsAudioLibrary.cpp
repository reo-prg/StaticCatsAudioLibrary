// StaticCatsAudioLibrary.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "../include/StaticCatsAudioLibrary.h"
#include "../include/SCAL_Listener.h"
#include "../include/SCAL_Loader.h"
#include "../include/SCAL_Sound.h"
#include "../include/SCAL_Utility.h"
#include <memory>
#include <vector>
#include <x3daudio.h>



#define FAIL_RETURN(result) if(FAILED(result)){ return; }
#define FAIL_RETURN_FALSE(result) if(FAILED(result)){ return false; }


namespace scal
{
	constexpr float defaultSoundSpeed = 340.0f;

	struct SystemSetting
	{
		bool use3DSound_ = false;
		float soundSpeed_ = defaultSoundSpeed;

		EulerOrder order_ = EulerOrder::XYZ;

		float update3dInterval_ = 0.05f;
	};

	struct SystemState
	{
		bool isAvailable_ = false;
	};

	SystemSetting sys_setting;
	SystemState sys_state;

	class AudioManager final
	{
	public:
		AudioManager() {};
		~AudioManager() 
		{
			if (xaudioCore_)
			{
				Terminate();
			}
		};

		bool Initialize(void)
		{
			HRESULT result;

			// IXAudio2オブジェクトの作成
			result = XAudio2Create(&xaudioCore_);
			FAIL_RETURN_FALSE(result)

				// デバッグの設定
#ifdef _DEBUG
				XAUDIO2_DEBUG_CONFIGURATION debugc = {};
			debugc.TraceMask = XAUDIO2_LOG_WARNINGS;
			debugc.BreakMask = XAUDIO2_LOG_ERRORS;
			xaudioCore_->SetDebugConfiguration(&debugc);
#endif

			// マスタリングボイスの作成
			result = xaudioCore_->CreateMasteringVoice(&masterVoice_, XAUDIO2_DEFAULT_CHANNELS,
				XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, nullptr);
			FAIL_RETURN_FALSE(result);

			masterVoice_->GetVoiceDetails(&masterVoiceDetails_);

			return true;
		}

		void Terminate(void)
		{
			if (masterVoice_)
			{
				masterVoice_->DestroyVoice();
				masterVoice_ = nullptr;
			}
			
			xaudioCore_->Release();
			xaudioCore_ = nullptr;
		}

		IXAudio2* xaudioCore_ = nullptr;
		IXAudio2MasteringVoice* masterVoice_ = nullptr;
		XAUDIO2_VOICE_DETAILS masterVoiceDetails_ = {};
	};

	AudioManager manager;


	class Audio3DCalculator
	{
	public:
		Audio3DCalculator() = default;
		~Audio3DCalculator() 
		{ 
			if (isAvailable_)
			{
				Terminate();
			}
		}

		bool Initialize(const SystemSetting& sys_setting)
		{
			if (!sys_setting.use3DSound_) { return true; }

			HRESULT result;

			DWORD channel;
			manager.masterVoice_->GetChannelMask(&channel);
			result = X3DAudioInitialize(channel, sys_setting.soundSpeed_, x3dHandle_);
			if (FAILED(result))
			{
				OutputDebugStringA("SCAL_ERROR : Cant Initialize 3DAudio\n");
				return false;
			}

			isAvailable_ = true;
			return true;
		}

		void Terminate(void)
		{
			

			isAvailable_ = false;
		}

		void Register(Listener* listener)
		{
			listener_.push_back(listener);
		}
		void Register(SoundEmitter* emitter)
		{
			emitter_.push_back(emitter);
		}

		void Unregister(Listener* listener)
		{
			std::erase_if(listener_,
				[&listener](Listener* l) { return l == listener; });
		}

		void Unregister(SoundEmitter* emitter)
		{
			std::erase_if(emitter_,
				[&emitter](SoundEmitter* e) { return e == emitter; });
		}

		void Update(float delta)
		{
			updateTimer_ += delta;

			if (updateTimer_ < sys_setting.update3dInterval_)
			{
				return;
			}

			updateTimer_ -= sys_setting.update3dInterval_;

			for (auto& l : listener_)
			{
				auto&& rl = l->GetRawListener();

				for (auto& e : emitter_)
				{
					e->Calculate(x3dHandle_, &rl);
				}
			}
		}
	private:
		X3DAUDIO_HANDLE x3dHandle_;
		bool isAvailable_ = false;

		std::vector<Listener*> listener_;
		std::vector<SoundEmitter*> emitter_;

		float updateTimer_ = 0.0f;
	};

	Audio3DCalculator x3dManager;


	WAVLoader wavLoader;


	bool Initialize(void)
	{
		bool result;

		result = manager.Initialize();
		if (!result) { return false; }

		result = x3dManager.Initialize(sys_setting);
		if (!result) { return false; }

		sys_state.isAvailable_ = true;

		return true;
	}

	void Terminate(void)
	{
		sys_state.isAvailable_ = false;

		wavLoader.Terminate();
		x3dManager.Terminate();
		manager.Terminate();
	}

	void SetUse3DSound(bool using3DSound)
	{
		if (sys_state.isAvailable_) { return; }

		sys_setting.use3DSound_ = using3DSound;
	}

	bool LoadSoundFile(const std::string& filepath)
	{
		std::string ext = scal_util::GetExtension(filepath);

		if (ext == "wav")
		{
			if (!wavLoader.LoadWAVFile(filepath))
			{
				return false;
			}
		}
		else if (ext.empty())
		{
			if (!wavLoader.LoadWAVFile(filepath + ".wav"))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	bool LoadSoundFile(unsigned char* rawdata, unsigned int size, const std::string& key)
	{
		return wavLoader.LoadWAVFile(rawdata, size, key);
	}

	SoundData GetSoundData(const std::string& filepath)
	{
		SoundData data = {};

		auto&& d = wavLoader.GetWAVFile(filepath);
		if (!d) 
		{ 
			wavLoader.LoadWAVFile(filepath); 
			d = wavLoader.GetWAVFile(filepath);
		}

		if (d)
		{
			data.dataSize_ = d->dataSize_;
			data.data_ = d->data_;
			data.fileSize_ = d->fileSize_;
			data.fmt_ = d->fmt_;
		}
		return data;
	}

	SoundData GetSoundData(unsigned char* rawdata, int size, std::string& key)
	{
		SoundData data = {};

		auto&& d = wavLoader.GetWAVFile(key);
		if (!d)
		{
			wavLoader.LoadWAVFile(rawdata, size, key);
			d = wavLoader.GetWAVFile(key);
		}

		data.dataSize_ = d->dataSize_;
		data.data_ = d->data_;
		data.fileSize_ = d->fileSize_;
		data.fmt_ = d->fmt_;
		return data;
	}

	void DestroySoundFile(const std::string& filepath)
	{
		std::string ext = scal_util::GetExtension(filepath);

		if (ext == "wav")
		{
			wavLoader.DestroyWAVFile(filepath);
			return;
		}
		else if (ext.empty())
		{
			wavLoader.DestroyWAVFile(filepath + ".wav");
			return;
		}
		else
		{
			return;
		}
	}

	void RegisterEmitter(SoundEmitter* emitter)
	{
		x3dManager.Register(emitter);
	}

	void RegisterListener(Listener* listener)
	{
		x3dManager.Register(listener);
	}

	void UnregisterEmitter(SoundEmitter* emitter)
	{
		x3dManager.Unregister(emitter);
	}

	void UnregisterListener(Listener* listener)
	{
		x3dManager.Unregister(listener);
	}

	void Update3DAudio(float delta)
	{
		x3dManager.Update(delta);
	}

	void SetUpdateInterval(float interval)
	{
		sys_setting.update3dInterval_ = interval;
	}


	WAVLoader& GetWavLoader(void)
	{
		return wavLoader;
	}

	IXAudio2* GetXAudio2Core(void)
	{
		return manager.xaudioCore_;
	}
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void)
	{
		return manager.masterVoiceDetails_;
	}
	IXAudio2MasteringVoice*& GetMasterVoice(void)
	{
		return manager.masterVoice_;
	}
	void SetEulerOrder(EulerOrder order)
	{
		sys_setting.order_ = order;
	}

	EulerOrder GetEulerOrder(void)
	{
		return sys_setting.order_;
	}
}