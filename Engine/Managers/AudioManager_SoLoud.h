#pragma once
#include "AudioManager.h"
#include <utility>
#include <unordered_map>
#include <string>

#include "soloud.h"
#include "soloud_wav.h"

namespace NomadEngine {
	class AudioManager_SoLoud
		: public AudioManager
	{
	public:
		AudioManager_SoLoud() {};
		~AudioManager_SoLoud() = default;

		bool Initialize() override;
		void Shutdown() override;

		bool LoadAudio(std::string audioName, std::string filepath) override;
		bool CheckAudioHandleIsValid(int audioHandle) override;

		int Play(std::string audioName) override;
		void Stop(int audioHandle) override;
		void Seek(int audioHandle, float time) override;
		void SetAudioLoop(int audioHandle, bool loop) override;
		void StopAll() override;
		void SetVolume(int audioHandle, float volume) override;
		float GetVolume(int audioHandle) override;

	private:
		SoLoud::Soloud soloud;
		std::unordered_map<std::string, SoLoud::Wav> audioAssets; // Possibly move to assetManager
		bool audioInitialized = false;
	};
}
