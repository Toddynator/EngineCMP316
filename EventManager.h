#pragma once
#include "Manager.h"

namespace CMP316engine {
	class EventManager : Manager
	{
	public:
		virtual bool processEvents() = 0;
	};

	enum SystemEvent_Type
	{
		// May instead want this in specific managers / systems. e.g. window events.
		// For now I can maybe just define only the events I currently need.
	};

	struct SystemEvents
	{
		SystemEvent_Type type;
	};
}

