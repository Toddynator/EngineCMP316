/*
Using SDL3 to handle window creation but DirectX11 for rendering. Removes a lot of the boilerplate of using purely directx11.
Also partly because directx11 is being used in other course modules as far as I'm aware.
*/

#include <iostream>
#include <Windows.h>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "d3dclass.h" // RENDERER

/// WINDOW SETTINGS ///

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

///

static D3DClass* renderer = NULL;
static SDL_Window* window = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /// WINDOW ///

    window = SDL_CreateWindow("CMP316 Engine", 800, 600, NULL);
    if(!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
	}
    
    /// ADJUST WINDOW SETTINGS ///

	SDL_SetWindowFullscreen(window, FULL_SCREEN ? SDL_WINDOW_FULLSCREEN : 0);

    /// RENDERER  ///

    renderer = new D3DClass;
    
    int screenWidth, screenHeight;
	screenWidth = 0; screenHeight = 0;  
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

    if (!renderer->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	//renderer->BeginScene(0.5f, 0.5f, 0.5f, 1.0f); // Grey
    renderer->BeginScene(0.5f, 0.5f, 0.0f, 1.0f); // Yellow

    // Scene contained here...

	renderer->EndScene();
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // Release the Direct3D object.
    if (renderer)
    {
        renderer->Shutdown();
        delete renderer;
        renderer = NULL;
    }
}

