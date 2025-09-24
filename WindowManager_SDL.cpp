#include "WindowManager_SDL.h"

bool CMP316engine::WindowManager_SDL::Initialize()
{
	return createWindow();
}

void CMP316engine::WindowManager_SDL::Shutdown()
{
	SDL_DestroyWindow(window);
}

bool CMP316engine::WindowManager_SDL::createWindow()
{
	/// FLAGS: https://wiki.libsdl.org/SDL3/SDL_WindowFlags
	SDL_WindowFlags flags{};
	flags |= SDL_WINDOW_RESIZABLE;
	if (FULL_SCREEN) { flags |= SDL_WINDOW_FULLSCREEN; }
	///
	window = SDL_CreateWindow("CMP316 Engine", 800, 600, flags);
	if (!window) {
		SDL_Log("Couldn't create window: %s", SDL_GetError());
		return false;
	}
	return true;
}

void CMP316engine::WindowManager_SDL::fullscreenWindow()
{
	FULL_SCREEN = !FULL_SCREEN;
	SDL_SetWindowFullscreen(window, FULL_SCREEN);
}

HWND CMP316engine::WindowManager_SDL::getHWND()
{
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
	return (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
}
