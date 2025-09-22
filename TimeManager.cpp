#include "TimeManager.h"

TimeManager::TimeManager()
{
	deltaTime = 0.0f;
	lastUpdateTime = std::chrono::steady_clock::now();
}

void TimeManager::Update()
{
	auto now = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(now - lastUpdateTime).count();
	lastUpdateTime = now;
}
