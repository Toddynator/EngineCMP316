/*
ABSTRACT CLASS
Abstracted audio incase I want to swap out SoLoud for a more powerful library.
*/

#pragma once
#include "Manager.h"
#include <string>

namespace CMP316engine {
	class AudioManager :
		public Manager
	{
	public:
		virtual bool Initialize() { return true; }

		virtual bool LoadAudio(std::string audioName, std::string filepath) = 0;
		// Should check if audio handle is still valid before using it.
		virtual bool CheckAudioHandleIsValid(int audioHandle) = 0;

		// @param name of the the type of audio to play
		// @return returns the index of the audio of a given type
		virtual int Play(std::string audioName) = 0;
		virtual void Stop(int audioHandle) = 0;
		virtual void Seek(int audioHandle, float time) = 0;
		virtual void SetAudioLoop(int audioHandle, bool loop) = 0;
		virtual void StopAll() = 0;		
		// Volume from 0 to 1
		virtual void SetVolume(int audioHandle, float volume) = 0;
		virtual float GetVolume(int audioHandle) = 0;
	};
}

