#pragma once
#include "WindowManager.h"
#include <SDL3/SDL.h>

/*
It is unlikely I will change out SDL for any other window creation, as SDL already handles abstraction for different platforms.
I've created a thin wrapper just to provide some abstraction from the rest of the engine. But I'll need to either go all the way
and fully abstract it at some point or get rid of the abstraction layer completely.
*/

namespace NomadEngine {
	class WindowManager_SDL
		: public WindowManager
	{
		bool Initialize() override;
		void Shutdown() override;

		bool createWindow(const WindowProperties windowProperties = WindowProperties()) override;

		void FullscreenWindow() override;
		virtual void GetWindowSize(int& width, int& height);

		void* GetNativeWindow() const override { return window; }
		HWND GetHWND() const override;

	private:
		SDL_Window* window;
	};
}
