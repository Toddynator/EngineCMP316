/*
Using SDL3 to handle window creation but DirectX11 for rendering. Removes a lot of the boilerplate of using purely directx11.
Also partly because directx11 is being used in other course modules as far as I'm aware.

NOTE:
- SDL uses its own main function as the entry point of the program, so the program entry point is SDL_main() (Or something like that) instead of this file.

TODO:
- Sort out my priorities / todo list
- Create helper for matrix transformations (Translation, Rotation, Scale)
- Create Base Object class that has position, rotation and scale. ModelClass should only handle the model data and rendering ~ REMOVE TRANSFORMS FROM MODEL CLASS, remember to remove the test transforms in the render() function.
- Input Manager: (Detecting specific keys on a keyboard) 
https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed 
- Currently program ends on any key press, should remove this and instead toggle fullscreen when 'F11' is pressed so that you can press the X button to close the program instead.
- Go through the core files and add comments / documentation explaining purpose of each class and function, and what they can be used for.
- Possibly encapsulate the rasterizer in d3dclass.cpp
- Encapsulate the scene/frame logic in a separate class (Application class?) ~ Move model, camera and colorshader to this class. Renderer should probably move into that class as well.
- Possibly update ColorShaderClass::Initialize so that the filepath to the vertex and pixel shader is calculated based on local 
directory rather than hard coded based on the name. Currently Dependant on project folder being named exactly 'EngineCMP316'.
- Add Support for mixed texture and colour vertices.
- Projection Matrix I think should be under camera class itself in the future, need to research this more to make sure I don't break everything. If I do this I can then enable support for multiple cameras.
  - REMEMBER TO MOVE THE PROJECTION MATRIX FROM D3DCLASS IF I DO THIS, PROJECTION MATRIX MUST BE RECALCULATED AFTER WINDOW RESIZE (CHECK D3DCLASS HANDLEWINDOWRESIZE FUNCTION).

INPUTMANAGER TODO:
- Add Mouse Functionality.
- Unit Test & Profiling.
*/

#include <iostream>
#include <Windows.h>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <../ImGui/imgui.h>
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_dx11.h"

#include "d3dclass.h" // RENDERER
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "InputManager.h"

/// WINDOW SETTINGS ///

bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

///

static SDL_Window* window = NULL;
static D3DClass* renderer = NULL;
CameraClass* m_Camera = NULL;
ModelClass* m_Model = NULL;
ColorShaderClass* m_ColorShader = NULL;
TextureShaderClass* m_TextureShader = NULL;
InputManager* inputManager = new InputManager{};

///

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /// TEXTURES ///
    char textureFilename[128];

    /// WINDOW ///
    /// FLAGS: https://wiki.libsdl.org/SDL3/SDL_WindowFlags
    SDL_WindowFlags flags{};
    flags |= SDL_WINDOW_RESIZABLE;
	if (FULL_SCREEN) { flags |= SDL_WINDOW_FULLSCREEN; }
    ///
    window = SDL_CreateWindow("CMP316 Engine", 800, 600, flags);
    if(!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
	}
    
    /// ADJUST WINDOW SETTINGS ///

	//SDL_SetWindowFullscreen(window, FULL_SCREEN);

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

    // Create the camera object.
    m_Camera = new CameraClass;
    // Set the initial position of the camera.
    m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

    // Create and initialize the model object.
    m_Model = new ModelClass;

	// Set the name of the texture file that we will be loading.
	strcpy_s(textureFilename, "../EngineCMP316/data/stone01.tga");
    //strcpy_s(textureFilename, "../EngineCMP316/data/testTargaFile.tga");
    //strcpy_s(textureFilename, "../EngineCMP316/data/test24bitTargaFile.tga");

    //if (!m_Model->Initialize(renderer->GetDevice(), renderer->GetDeviceContext())) // Without Texture
    if (!m_Model->Initialize(renderer->GetDevice(), renderer->GetDeviceContext(), textureFilename))
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return SDL_APP_FAILURE;
    }

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	if (!m_TextureShader->Initialize(renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return SDL_APP_FAILURE;
	}

	/// IMGUI ///

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer back-ends
	ImGui_ImplSDL3_InitForD3D(window);
	ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetDeviceContext());

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, key presses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	//////////////////
	/// QUIT EVENT ///

	if (event->type == SDL_EVENT_QUIT)
	{
		// WITHOUT THIS, YOU WON'T BE ABLE TO CLOSE THE WINDOW WHEN ALT+F4 IS PRESSED OR THE X BUTTON IS CLICKED.
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}

    //////////////////////////////
	/// WINDOW RESIZE HANDLING ///

    if (event->type == SDL_EVENT_WINDOW_RESIZED)
	{
		// Have to update the renderer when the window resizes, otherwise the scene will appear distorted/stretched.
		int screenWidth, screenHeight;
		SDL_GetWindowSize(window, &screenWidth, &screenHeight);
		renderer->HandleWindowResize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
    } 

	/////////////
	/// INPUT ///

	ImGui_ImplSDL3_ProcessEvent(event);  
    inputManager->updateInputStates(event);

    /////////////

    return SDL_APP_CONTINUE;
}



/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    /* Possible Structure:
	application->updateInputs();
	application->updateImGui();
	application->updateRender();
    inputManager->EndFrame();
    */

    ///// INPUT

    if (inputManager->IsKeyPressed(SDL_SCANCODE_LSHIFT)) { std::cout << "\nShift was pressed, inputManager did its job!!"; }
    if (inputManager->IsKeyPressed(SDL_SCANCODE_F11)) { 
        FULL_SCREEN = !FULL_SCREEN; 
        SDL_SetWindowFullscreen(window, FULL_SCREEN);
    }
    if (inputManager->IsMouseButtonPressed(SDL_BUTTON_LEFT)) { std::cout << "\nLeft Mouse was pressed, inputManager did its job!!"; }
    if (inputManager->IsMouseButtonReleased(SDL_BUTTON_LEFT)) { std::cout << "\nLeft Mouse was released, inputManager did its job!!"; }

    inputManager->EndFrame(); // Should move this to the very end, just in case maybe the update loop for whatever reason has input calls for example.

    ///// IMGUI  

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0U, (const ImGuiViewport *)0, ImGuiDockNodeFlags_PassthruCentralNode); // Supports docking windows to the viewport, must be rendered before other ImGui Windows
	ImGui::ShowDemoWindow(); 

	/////

    XMMATRIX viewMatrix, projectionMatrix;
    renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f); // Black

    /////

    // Generate the view matrix based on the camera's position.
    m_Camera->Render();

    // Get the view, and projection matrices from the camera and d3d objects.
    m_Camera->GetViewMatrix(viewMatrix);
    renderer->GetProjectionMatrix(projectionMatrix);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    // CALL THIS FOR EACH OBJECT IN THE SCENE
    m_Model->Render(renderer->GetDeviceContext());

	// Render the model using the texture shader.
    // CALL THIS FOR EACH OBJECT IN THE SCENE
	if (!m_TextureShader->Render(renderer->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->GetWorldMatrix(), viewMatrix, projectionMatrix, m_Model->GetTexture()))
	{
		return SDL_APP_FAILURE;
	}

    /////

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    /////

	renderer->EndScene();
    return SDL_APP_CONTINUE;
}



/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    delete inputManager;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

    // Release the color shader object.
    if (m_ColorShader)
    {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = NULL;
    }

    // Release the model object.
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = NULL;
    }

    // Release the camera object.
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = NULL;
    }

    // Release the Direct3D object.
    if (renderer)
    {
        renderer->Shutdown();
        delete renderer;
        renderer = NULL;
    }
}

