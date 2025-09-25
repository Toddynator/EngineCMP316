#pragma once
#include "WindowManager.h"
#include <SDL3/SDL.h>

/*
It is unlikely I will change out SDL for any other window creation, as SDL already handles abstraction for different platforms.
I've created this thin wrapper just to provide some abstraction from the rest of the engine.
*/

namespace CMP316engine {
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
