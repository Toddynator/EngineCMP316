#include "pch.h"
#include "AudioManager_SoLoud.h"

/*
NOTE: Some of the University computers don't have audio drivers so I want my program to still run without audio, hence I don't halt the program completely.
*/

bool NomadEngine::AudioManager_SoLoud::Initialize()
{
	// Initialize Audio Library
	SoLoud::result result;
	result = soloud.init();
	if (result != 0) { 
		std::cout << "\nAudio Init Error Result: " << result << "\n";
		audioInitialized = false;
		return true;
	}
	else {
		audioInitialized = true;
	}
	

	// Load Default Audio Files
	if (!LoadAudio("MyJam", "data/Audio/9 (102 BPM)_Seq02.wav")) { return false; }

	return true;
}

void NomadEngine::AudioManager_SoLoud::Shutdown()
{
	if (!audioInitialized) { return; }
	soloud.deinit();
}

bool NomadEngine::AudioManager_SoLoud::LoadAudio(std::string audioName, std::string filepath)
{
	if (!audioInitialized) { return false; }

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

bool NomadEngine::AudioManager_SoLoud::CheckAudioHandleIsValid(int audioHandle)
{
	if (!audioInitialized) { return false; }
	return soloud.isValidVoiceHandle(audioHandle);
}

int NomadEngine::AudioManager_SoLoud::Play(std::string audioName)
{
	if (!audioInitialized) { return 0; }
	SoLoud::handle audioHandle = soloud.play(audioAssets[audioName]);
	return audioHandle;
}

void NomadEngine::AudioManager_SoLoud::Stop(int audioHandle)
{
	if (!audioInitialized) { return; }
	soloud.setLooping(audioHandle, true);
}

void NomadEngine::AudioManager_SoLoud::Seek(int audioHandle, float time)
{
	if (!audioInitialized) { return; }
	soloud.seek(audioHandle, time);
}

void NomadEngine::AudioManager_SoLoud::SetAudioLoop(int audioHandle, bool loop)
{
	if (!audioInitialized) { return; }
	soloud.setLooping(audioHandle, loop);
}

void NomadEngine::AudioManager_SoLoud::StopAll()
{
	if (!audioInitialized) { return; }
	soloud.stopAll();
}

void NomadEngine::AudioManager_SoLoud::SetVolume(int audioHandle, float volume)
{
	if (!audioInitialized) { return; }
	soloud.setVolume(audioHandle, volume);
}

float NomadEngine::AudioManager_SoLoud::GetVolume(int audioHandle)
{
	if (!audioInitialized) { return 0.f; }
	return soloud.getVolume(audioHandle);
}
