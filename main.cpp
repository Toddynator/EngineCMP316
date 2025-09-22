/*
MANAGERS TODO:
- WindowManager
* Should contain the window and abstract window creation from any particular API. 
* Should be able to return HWND on windows (e.g. for Dx11 Renderer).
* Should be able to modify the window, e.g. resizeWindow() or fullscreenWindow()

TODO:
- Use CMP316engine Namespace around all the managers.
- Make all raw pointers in EngineLayer a smart pointer / unique pointer.
- Create unified shader class that supports texture and colour, colour should multiply the colour of the texture.
* Remove texture and colour shader class after this is complete.
- Create WindowManager so that the window logic is abstracted and the engine is not Dependant on SDL.
- Create EventManager so that the event logic is no longer Dependant on SDL.
- Create RendererManager so that the renderer logic is abstracted and the engine is not Dependant on DirectX11.
- Create helper for matrix transformations (Translation, Rotation, Scale)
- Create Base Object class that has position, rotation and scale. ModelClass should only handle the model data and rendering ~ REMOVE TRANSFORMS FROM MODEL CLASS, remember to remove the test transforms in the render() function.
- Go through the core files and add comments / documentation explaining purpose of each class and function, and what they can be used for.
- Enforce naming convention on all files.
- Put view matrix under the camera class.

INPUTMANAGER TODO:
- Abstract from SDL, map SDL scan codes to my own input enum, use my own event system for updating the key states.
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