#pragma once
#include "EventManager.h"

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

