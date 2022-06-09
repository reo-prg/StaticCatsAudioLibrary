// StaticCatsAudioLibrary.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "../include/StaticCatsAudioLibrary.h"
#include "../include/SCAL_Loader.h"
#include "../include/SCAL_Utility.h"
#include <memory>



#define FAIL_RETURN(result) if(FAILED(result)){ return; }
#define FAIL_RETURN_FALSE(result) if(FAILED(result)){ return false; }


namespace scal
{
	class AudioManager;

	std::unique_ptr<AudioManager> manager;
	std::unique_ptr<WAVLoader> wavLoader;




	class AudioManager final
	{
	public:
		AudioManager() {};
		~AudioManager() {};

		bool Initialize(void)
		{
			HRESULT result;

			wavLoader.reset(new WAVLoader());

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


			return true;
		}

		IXAudio2* xaudioCore_;
		IXAudio2MasteringVoice* masterVoice_;
		XAUDIO2_VOICE_DETAILS masterVoiceDetails_ = {};
	};




	bool Initialize(void)
	{
		bool result;

		manager = std::make_unique<AudioManager>();
		result = manager->Initialize();
		if (!result) { return false; }


		return true;
	}

	bool LoadSoundFile(const std::string& filepath)
	{
		std::string ext = scal_util::GetExtension(filepath);

		if (ext == "wav")
		{
			if (!wavLoader->LoadWAVFile(filepath))
			{
				return false;
			}
		}
		else if (ext.empty())
		{
			if (!wavLoader->LoadWAVFile(filepath + ".wav"))
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
			auto&& d = wavLoader->GetWAVFile(filepath);
			data.dataSize_ = d->dataSize_;
			data.data_ = d->data_;
			data.fileSize_ = d->fileSize_;
			data.fmt_ = d->fmt_;
			return data;
		}
		else if (ext.empty())
		{
			auto&& d = wavLoader->GetWAVFile(filepath + ".wav");
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
			wavLoader->DestroyWAVFile(filepath);
			return;
		}
		else if (ext.empty())
		{
			wavLoader->DestroyWAVFile(filepath + ".wav");
			return;
		}
		else
		{
			return;
		}
	}


	WAVLoader& GetWavLoader(void)
	{
		return *wavLoader;
	}

	IXAudio2* GetXAudio2Core(void)
	{
		return manager->xaudioCore_;
	}
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void)
	{
		return manager->masterVoiceDetails_;
	}
}