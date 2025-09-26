#include "EngineLayer.h"



bool EngineLayer::Initialize()
{
	///////////////////
	/// APPLICATION ///

	application = std::make_unique<TempApplication>();
	if (!application->Initialize()) { return false; }

	/////////////////////
	/// EVENT MANAGER ///

	eventManager = std::make_unique<CMP316engine::EventManager_SDL>();
	eventManager->Initialize();

	/////////////////////
	/// TIME MANAGGER ///

	timeManager = std::make_unique<CMP316engine::TimeManager>();

	/////////////////////
	/// INPUT MANAGER ///

	inputManager = std::make_unique<CMP316engine::InputManager>();

	//////////////
	/// WINDOW ///

	windowManager = std::make_unique<CMP316engine::WindowManager_SDL>();
	if (!windowManager->Initialize()) { return false; }
	HWND hwnd = windowManager->GetHWND();

	/////////////////
	/// RENDERER  ///

	if (!createRenderer(hwnd)) {
		return false;
	}

	/////////////
	/// IMGUI ///

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer back-ends
	ImGui_ImplSDL3_InitForD3D(static_cast<SDL_Window*>(windowManager->GetNativeWindow()));
	ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetDeviceContext());

	/////////////
	/// SCENE ///

	// Create and initialize the texture shader object.
	shader = std::make_unique<TextureShaderClass>();

	if (!shader->Initialize(renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = std::make_unique<CameraClass>();
	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	model = std::make_unique<ModelClass>();

	// Set the name of the texture file that we will be loading.
	char textureFilename[128];
	std::filesystem::path filepath = std::filesystem::current_path();
	std::string assetFilepath = filepath.string() + "/data/stone01.tga";
	strcpy_s(textureFilename, assetFilepath.c_str());

	if (!model->Initialize(renderer->GetDevice(), renderer->GetDeviceContext(), textureFilename))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void EngineLayer::Run()
{
	while (true)
	{
		if (!processEvents()) { return; }
		Update();
		Render();
	}
}

void EngineLayer::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (application) { application->Shutdown(); }
	if (shader) { shader->Shutdown(); }
	if (model) { model->Shutdown(); }
	if (renderer) { renderer->Shutdown(); }
	if (windowManager) { windowManager->Shutdown(); }
}

bool EngineLayer::processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0)
	{
		//////////////////
		/// QUIT EVENT ///

		if (event.type == SDL_EVENT_QUIT)
		{
			// WITHOUT THIS, YOU WON'T BE ABLE TO CLOSE THE WINDOW WHEN ALT+F4 IS PRESSED OR THE X BUTTON IS CLICKED.
			return false;  /* end the program, reporting success to the OS. */
		}

		//////////////////////////////
		/// WINDOW RESIZE HANDLING ///

		if (event.type == SDL_EVENT_WINDOW_RESIZED)
		{
			// Have to update the renderer when the window resizes, otherwise the scene will appear distorted/stretched.
			int screenWidth, screenHeight;
			windowManager->GetWindowSize(screenWidth, screenHeight);
			renderer->HandleWindowResize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
		}

		/////////////
		/// INPUT ///

		ImGui_ImplSDL3_ProcessEvent(&event);
		inputManager->updateInputStates(&event);
	}
	return true;
}

void EngineLayer::Update()
{
	///// TIME

	timeManager->Update();

	///// INPUT

	// TODO: Make a global inputs function for encapsulating application input
	if (inputManager->IsKeyPressed(SDL_SCANCODE_F11)) {
		windowManager->FullscreenWindow();
	}
	inputManager->EndFrame(); // Should move this to the very end, just in case maybe the update loop for whatever reason has input calls for example.

	///// IMGUI  

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0U, (const ImGuiViewport*)0, ImGuiDockNodeFlags_PassthruCentralNode); // Supports docking windows to the viewport, must be rendered before other ImGui Windows
	ImGui::ShowDemoWindow();

	///// SCENE

	application->HandleInput();
	application->HandleImgui();
	application->Update(timeManager->getDeltaTime());
}

void EngineLayer::Render()
{
	XMMATRIX viewMatrix, projectionMatrix;
	renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f); // Black

	///// SCENE

	application->Render();

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	renderer->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// CALL THIS FOR EACH OBJECT IN THE SCENE
	model->Render(renderer->GetDeviceContext());

	// Render the model using the texture shader.
	// CALL THIS FOR EACH OBJECT IN THE SCENE
	if (!shader->Render(renderer->GetDeviceContext(), model->GetIndexCount(), model->GetWorldMatrix(), viewMatrix, projectionMatrix, model->GetTexture()))
	{
		return;
	}

	///// IMGUI

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	/////

	renderer->EndScene();
}

bool EngineLayer::createRenderer(HWND hwnd)
{
	renderer = std::make_unique<D3DClass>();

	int screenWidth, screenHeight;
	screenWidth = 0; screenHeight = 0;
	windowManager->GetWindowSize(screenWidth, screenHeight);

	if (!renderer->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, windowManager->IsFullscreen(), SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}
