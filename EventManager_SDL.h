#pragma once
#include "EventManager.h"

namespace CMP316engine {
	class EventManager_SDL : 
		public EventManager
	{
	public:
		bool Initialize();
		void Shutdown();

		bool processEvents();
	};
}

