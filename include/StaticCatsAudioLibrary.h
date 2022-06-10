#pragma once
#include <xaudio2.h>
#include <string>
#include "SCAL_Loader.h"

#pragma comment(lib,"xaudio2.lib")

namespace scal
{
	enum class SoundState
	{
		Playing,
		Stop,
	};

	struct SoundData
	{
		unsigned int fileSize_;
		FmtDesc fmt_;
		unsigned int dataSize_;
		unsigned char* data_;
	};

	class AudioManager;
	class WAVLoader;


	bool Initialize(void);
	void Terminate(void);

	void SetUse3DSound(bool using3DSound);


	bool LoadSoundFile(const std::string& filepath);
	SoundData GetSoundData(const std::string& filepath);
	void DestroySoundFile(const std::string& filepath);



	WAVLoader& GetWavLoader(void);

	IXAudio2* GetXAudio2Core(void);
	XAUDIO2_VOICE_DETAILS GetMasterDetails(void);
}
