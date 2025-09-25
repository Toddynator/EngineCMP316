#include "EventManager_SDL.h"
#include <SDL3/SDL.h>

bool CMP316engine::EventManager_SDL::PollEvent(Event& event)
{
	if (SDL_PollEvent(&sdlEvent))
	{
		translateEvent();
		return true;
	}
	return false;
}

CMP316engine::Event CMP316engine::EventManager_SDL::translateEvent()
{
	
}
