#include "pch.h"
#include "EngineLayer.h"



bool EngineLayer::Initialize()
{
	///////////////////
	/// APPLICATION ///

	application = CMP316engine::CreateApp();
	if (!application->Initialize()) { return false; }

	/////////////////////
	/// TIME MANAGGER ///

	timeManager = std::make_unique<CMP316engine::TimeManager>();

	/////////////////////
	/// INPUT MANAGER ///

	inputManager = std::make_unique<CMP316engine::InputManager>();

	/////////////////////
	/// AUDIO MANAGER ///

	audioManager = std::make_unique<CMP316engine::AudioManager_SoLoud>();
	audioManager->Initialize();

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

	///////////////
	/// PHYSICS ///

	physicsManager = std::make_unique<CMP316engine::PhysicsManager>();
	physicsManager->Initialize();

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
	shader = std::make_unique<Shader>();

	if (!shader->Initialize(renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = std::make_unique<Camera>();
	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	model = std::make_unique<CMP316engine::Model>();

	if (!model->Initialize(renderer->GetDevice(), renderer->GetDeviceContext()))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/// PHYSICS TEST ///
	// We'll just associate this with our model for now
	JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
	modelPhysicsBodyID = physicsManager->GetBodyInterface().CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	physicsManager->GetBodyInterface().SetLinearVelocity(modelPhysicsBodyID, JPH::Vec3(0.1f, 0.1f, 0.0f));

	JPH::RVec3 position = physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	JPH::Vec3 velocity = physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	///


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

	if (physicsManager) { physicsManager->Shutdown(); }
	if (application) { application->Shutdown(); }
	if (shader) { shader->Shutdown(); }
	if (model) { model->Shutdown(); }
	if (renderer) { renderer->Shutdown(); }
	if (windowManager) { windowManager->Shutdown(); }
	if (audioManager) { audioManager->Shutdown(); }
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

	// TEST
	ImGui::Begin("Testing");
	if (ImGui::Checkbox("Wireframe", &wireframeEnabled))
	{
		renderer->ToggleWireframe();
	}
	model->RenderImGuiControls();
	ImGui::End();
	// TEST

	///// SCENE

	application->HandleInput();
	application->HandleImgui();
	application->Update(timeManager->getDeltaTime());

	/// AUDIO TEST
	if (!audioPlayed) {
		audioPlayed = true;
		int audioHandle = audioManager->Play("MyJam");
		audioManager->SetAudioLoop(audioHandle, true);
	}
	///

	/// PHYSICS TEST

	physicsManager->Update(timeManager->getDeltaTime());
	JPH::RVec3 position = physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	JPH::Vec3 velocity = physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	model->SetPosition(XMFLOAT3(position.GetX(), position.GetY(), position.GetZ()));

	///
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
	viewMatrix = camera->GetViewMatrix();
	projectionMatrix = renderer->GetProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// CALL THIS FOR EACH RENDERABLE OBJECT IN THE SCENE
	model->Render(shader.get(), renderer->GetDeviceContext(), viewMatrix, projectionMatrix);
	
	///// IMGUI

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	/////

	renderer->EndScene();
}

bool EngineLayer::createRenderer(HWND hwnd)
{
	renderer = std::make_unique<Renderer_DirectX11>();

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
