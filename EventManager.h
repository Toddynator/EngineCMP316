#pragma once
#include "Manager.h"

/*
Abstracts API events from engine events.
This event manager design will utilize the API's own event queue through the pollEvents() function,
it can then individually translate each event in the engines core class so that managers can then call their own processEvent functions.
This way the eventManager does not need to know about what uses the events, and the managers don't need to know about the API.
*/

namespace CMP316engine {	
	typedef union Event
	{
		//KeyboardEvent key;
		//WindowEvent window;
	};

	typedef struct KeyboardEvent
	{

	};

	typedef struct WindowEvent
	{

	};

	class EventManager : 
		public Manager
	{
	public:
		virtual bool PollEvent(Event& event) = 0;

	private:
		virtual Event translateEvent() = 0;
	};
}

