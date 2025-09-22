#include "EngineLayer.h"

EngineLayer::EngineLayer()
{

}

bool EngineLayer::Initialize()
{
	/// INPUT MANAGER ///

	inputManager = new InputManager{};

	/// TEXTURES ///
	char textureFilename[128];

	/// WINDOW ///

	if (!createWindow()) {
		return false;
	}
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
	HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	/// RENDERER  ///

	if (!createRenderer(hwnd)) {
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	m_Model = new ModelClass;

	// Set the name of the texture file that we will be loading.
	std::filesystem::path filepath = std::filesystem::current_path();
	std::string assetFilepath = filepath.string() + "/data/stone01.tga";
	strcpy_s(textureFilename, assetFilepath.c_str());

	if (!m_Model->Initialize(renderer->GetDevice(), renderer->GetDeviceContext(), textureFilename))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	if (!m_TextureShader->Initialize(renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
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

	return true;
}

void EngineLayer::Run()
{
	while (true)
	{
		///// EVENTS

		// Must be done first!
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (!pollEvent(&event))  /// Can make this one function!
			{
				return;
			}
		}

		///// INPUT

		// TEST INPUTS ///
		if (inputManager->IsKeyPressed(SDL_SCANCODE_LSHIFT)) { std::cout << "\nShift was pressed, inputManager did its job!!"; }
		if (inputManager->IsKeyPressed(SDL_SCANCODE_F11)) {
			FULL_SCREEN = !FULL_SCREEN;
			SDL_SetWindowFullscreen(window, FULL_SCREEN);
		}
		if (inputManager->IsMouseButtonPressed(SDL_BUTTON_LEFT)) { std::cout << "\nLeft Mouse was pressed, inputManager did its job!!"; }
		if (inputManager->IsMouseButtonReleased(SDL_BUTTON_LEFT)) { std::cout << "\nLeft Mouse was released, inputManager did its job!!"; }
		// TEST INPUTS ///

		inputManager->EndFrame(); // Should move this to the very end, just in case maybe the update loop for whatever reason has input calls for example.

		///// IMGUI  

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0U, (const ImGuiViewport*)0, ImGuiDockNodeFlags_PassthruCentralNode); // Supports docking windows to the viewport, must be rendered before other ImGui Windows
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
			return;
		}

		/////

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		/////

		renderer->EndScene();
	}
}

void EngineLayer::Shutdown()
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

bool EngineLayer::pollEvent(SDL_Event* event)
{
	//////////////////
	/// QUIT EVENT ///

	if (event->type == SDL_EVENT_QUIT)
	{
		// WITHOUT THIS, YOU WON'T BE ABLE TO CLOSE THE WINDOW WHEN ALT+F4 IS PRESSED OR THE X BUTTON IS CLICKED.
		return false;  /* end the program, reporting success to the OS. */
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

	return true;
}

bool EngineLayer::createWindow()
{
	/// FLAGS: https://wiki.libsdl.org/SDL3/SDL_WindowFlags
	SDL_WindowFlags flags{};
	flags |= SDL_WINDOW_RESIZABLE;
	if (FULL_SCREEN) { flags |= SDL_WINDOW_FULLSCREEN; }
	///
	window = SDL_CreateWindow("CMP316 Engine", 800, 600, flags);
	if (!window) {
		SDL_Log("Couldn't create window: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool EngineLayer::createRenderer(HWND hwnd)
{
	renderer = new D3DClass;

	int screenWidth, screenHeight;
	screenWidth = 0; screenHeight = 0;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	if (!renderer->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}
