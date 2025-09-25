#pragma once
#include "Manager.h"

namespace CMP316engine {
	class EventManager : 
		public Manager
	{
	public:
		virtual bool processEvents() = 0;
	};

	enum SystemEvent_Type
	{
		/// TODO GROUP INTO CATEGORIES, E.g. INPUT,WINDOW,SYSTEM,ETC ~ Look at game programming patterns page on events, might show a way to abstract this cleanly.
		// May instead want this in specific managers / systems. e.g. window events.
		// For now I can maybe just define only the events I currently need.

		
	};

	struct SystemEvents
	{
		SystemEvent_Type type;
	};
}

