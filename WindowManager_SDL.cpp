#include "WindowManager_SDL.h"

bool CMP316engine::WindowManager_SDL::Initialize()
{
	return createWindow();
}

void CMP316engine::WindowManager_SDL::Shutdown()
{
	SDL_DestroyWindow(window);
}

bool CMP316engine::WindowManager_SDL::createWindow(const WindowProperties windowProperties)
{
	/// FLAGS: https://wiki.libsdl.org/SDL3/SDL_WindowFlags
	SDL_WindowFlags flags{};
	flags |= SDL_WINDOW_RESIZABLE;
	if (fullscreen) { flags |= SDL_WINDOW_FULLSCREEN; }
	///
	window = SDL_CreateWindow(windowProperties.Title.c_str(), windowProperties.Width, windowProperties.Height, flags);
	if (!window) {
		SDL_Log("Couldn't create window: %s", SDL_GetError());
		return false;
	}
	return true;
}

void CMP316engine::WindowManager_SDL::FullscreenWindow()
{
	fullscreen = !fullscreen;
	SDL_SetWindowFullscreen(window, fullscreen);
}

void CMP316engine::WindowManager_SDL::GetWindowSize(int& width, int& height)
{
	SDL_GetWindowSize(window, &width, &height);
}

HWND CMP316engine::WindowManager_SDL::GetHWND() const
{
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
	return (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
}
