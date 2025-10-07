#include "pch.h"
#include "AudioManager_SoLoud.h"

bool CMP316engine::AudioManager_SoLoud::Initialize()
{
	// Initialize Audio Library
	SoLoud::result result;
	result = soloud.init();
	if (result != 0) { std::cout << "\nAudio Init Error Result: " << result; return false; }

	// Load Default Audio Files
	if (!LoadAudio("MyJam", "../data/Audio/9 (102 BPM)_Seq02.wav")) { return false; }

	return true;
}

void CMP316engine::AudioManager_SoLoud::Shutdown()
{
	soloud.deinit();
}

bool CMP316engine::AudioManager_SoLoud::LoadAudio(std::string audioName, std::string filepath)
{
	/// CHECK ASSET ALREADY EXISTS

	if (audioAssets.find(audioName) != audioAssets.end())
	{
		std::cout << "\nAudio: " << audioName << " already loaded";
		return false;
	}

	/// ATTEMPT LOAD INTO ASSETS

	SoLoud::result result = audioAssets[audioName].load(filepath.c_str());
	if (result != 0) 
	{ 
		std::cout << "\nAudio Wav Load Error Result: " << result << " for file: " << filepath;
		return false; 
	}

	return true;
}

bool CMP316engine::AudioManager_SoLoud::CheckAudioHandleIsValid(int audioHandle)
{
	return soloud.isValidVoiceHandle(audioHandle);
}

int CMP316engine::AudioManager_SoLoud::Play(std::string audioName)
{
	SoLoud::handle audioHandle = soloud.play(audioAssets[audioName]);
	return audioHandle;
}

void CMP316engine::AudioManager_SoLoud::Stop(int audioHandle)
{
	soloud.setLooping(audioHandle, true);
}

void CMP316engine::AudioManager_SoLoud::Seek(int audioHandle, float time)
{
	soloud.seek(audioHandle, time);
}

void CMP316engine::AudioManager_SoLoud::SetAudioLoop(int audioHandle, bool loop)
{
	soloud.setLooping(audioHandle, loop);
}

void CMP316engine::AudioManager_SoLoud::StopAll()
{
	soloud.stopAll();
}

void CMP316engine::AudioManager_SoLoud::SetVolume(int audioHandle, float volume)
{
	soloud.setVolume(audioHandle, volume);
}

float CMP316engine::AudioManager_SoLoud::GetVolume(int audioHandle)
{
	return soloud.getVolume(audioHandle);
}
