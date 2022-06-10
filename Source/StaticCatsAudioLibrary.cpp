// StaticCatsAudioLibrary.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "../include/StaticCatsAudioLibrary.h"
#include "../include/SCAL_Loader.h"
#include "../include/SCAL_Utility.h"
#include <memory>
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
	};

	struct SystemState
	{
		bool isAvailable_ = false;
	};

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
	private:
		X3DAUDIO_HANDLE x3dHandle_;
		bool isAvailable_ = false;
	};

	Audio3DCalculator x3dManager;


	WAVLoader wavLoader;

	SystemSetting sys_setting;
	SystemState sys_state;


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

	SoundData GetSoundData(const std::string& filepath)
	{
		std::string ext = scal_util::GetExtension(filepath);
		SoundData data = {};

		if (ext == "wav")
		{
			auto&& d = wavLoader.GetWAVFile(filepath);
			data.dataSize_ = d->dataSize_;
			data.data_ = d->data_;
			data.fileSize_ = d->fileSize_;
			data.fmt_ = d->fmt_;
			return data;
		}
		else if (ext.empty())
		{
			auto&& d = wavLoader.GetWAVFile(filepath + ".wav");
			data.dataSize_ = d->dataSize_;
			data.data_ = d->data_;
			data.fileSize_ = d->fileSize_;
			data.fmt_ = d->fmt_;
			return data;
		}
		else
		{
			return data;
		}
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
}