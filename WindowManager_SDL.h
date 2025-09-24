#pragma once
#include "WindowManager.h"
#include <SDL3/SDL.h>

namespace CMP316engine {
	class WindowManager_SDL
		: public WindowManager
	{
		bool Initialize() override;
		void Shutdown() override;

		bool createWindow() override;

		void fullscreenWindow() override;

		HWND getHWND() override;

	private:
		SDL_Window* window;
	};
}
