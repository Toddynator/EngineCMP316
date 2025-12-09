/*
The base game scene that loaded levels will use, this defines the custom systems made 
in the application and any systems in the engine that weren't added by default.
*/

#pragma once
#include "Core/Scenes/ECSScene.h"

class GameScene
	: public NomadEngine::ECSScene
{
public:
	GameScene(NomadEngine::EngineContext& context);

	bool Initialize() override;
};

