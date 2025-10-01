#pragma once
#include "Manager.h"
#include <chrono>

namespace CMP316engine {
	class TimeManager
		: public Manager

	{
	public:
		TimeManager();
		~TimeManager() = default;

		bool Initialize() { return true; }
		void Shutdown() {}

		void Update();

		float getDeltaTime() { return deltaTime; }

	private:
		float deltaTime; // Time between frames in seconds
		std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
	};
}
