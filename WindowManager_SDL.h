#pragma once
#include "WindowManager.h"
#include <SDL3/SDL.h>

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
