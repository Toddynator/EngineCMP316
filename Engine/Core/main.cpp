/*
Entry Point
*/

#include "pch.h"
#include "EngineLayer.h"

int main(int argc, char* argv[]) {
	std::unique_ptr<NomadEngine::EngineLayer> engine = std::make_unique<NomadEngine::EngineLayer>();
	if (engine->Initialize())
	{
		engine->Run();
	}
	engine->Shutdown();

	return 0;
}