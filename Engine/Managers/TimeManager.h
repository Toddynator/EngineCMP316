/*
Very simple class for calculating deltaTime.

TODO:
- Add a controllable fixed timestep for some sort of FixedUpdate() loop. Would be good for physics
updates for example.
*/

#pragma once
#include "Manager.h"
#include <chrono>

namespace NomadEngine {
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
		float GetFPS() { return 1.f / deltaTime; }

	private:
		float deltaTime; // Time between frames in seconds
		std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
	};
}
