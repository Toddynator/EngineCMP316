
/*
Currently unused until I figure out if I actually want to abstract events from SDL.
*/

#pragma once
#include "EventManager.h"
#include <SDL3/SDL.h>

namespace CMP316engine {
	class EventManager_SDL : 
		public EventManager
	{
	public:
		bool Initialize() { return true; }
		void Shutdown() {}

		bool PollEvent(Event& event) override;

	private:
		Event translateEvent() override;

	private:
		SDL_Event sdlEvent;
	};
}

