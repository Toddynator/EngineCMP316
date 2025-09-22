#pragma once
#include <chrono>

class TimeManager
{
public:
	TimeManager();
	~TimeManager() = default;

	void Update();

	float getDeltaTime() { return deltaTime; }

private:
	float deltaTime; // Time between frames in seconds
	std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
};

