/*
TODO:
- Create unified shader class that supports texture and colour, colour should multiply the colour of the texture.
* Remove texture and colour shader class after this is complete.
- Create WindowManager so that the window logic is abstracted and the engine is not Dependant on SDL.
- Create RendererManager so that the renderer logic is abstracted and the engine is not Dependant on DirectX11.
- Create helper for matrix transformations (Translation, Rotation, Scale)
- Create Base Object class that has position, rotation and scale. ModelClass should only handle the model data and rendering ~ REMOVE TRANSFORMS FROM MODEL CLASS, remember to remove the test transforms in the render() function.
- Go through the core files and add comments / documentation explaining purpose of each class and function, and what they can be used for.
- Put view matrix under the camera class.

INPUTMANAGER TODO:
- Add Mouse Functionality.
- Unit Test & Profiling.
*/

///

#include <memory.h>

#include "EngineLayer.h"

///

int main(int argc, char* argv[]) {
	std::unique_ptr<EngineLayer> engine = std::make_unique<EngineLayer>();
	if (engine->Initialize())
	{
		engine->Run();
	}
	engine->Shutdown();

	return 0;
}