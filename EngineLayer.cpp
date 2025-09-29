#include "pch.h"
#include "EngineLayer.h"



bool EngineLayer::Initialize()
{
	///////////////////
	/// APPLICATION ///

	application = std::make_unique<TempApplication>();
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

	/// TEST AUDIO
	if (!audioPlayed) {
		audioPlayed = true;
		int audioHandle = audioManager->Play("MyJam");
		audioManager->SetAudioLoop(audioHandle, true);
	}
	///
	
	/// TEST PHYSICS
	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
	// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
	// malloc / free.
	//JPH::TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	//JPH::JobSystemThreadPool job_system(2048, 8, thread::hardware_concurrency() - 1);

	//JPH::PhysicsSystem physics_system;
	// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
	// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
	//JPH::BodyInterface& body_interface = physics_system.GetBodyInterface();

	// Now create a dynamic body to bounce on the floor
	// Note that this uses the shorthand version of creating and adding a body to the world
	//JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 2.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
	//JPH::BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	/*
	// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
	// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
	body_interface.SetLinearVelocity(sphere_id, JPH::Vec3(0.0f, -5.0f, 0.0f));

	JPH::uint step = 0;
	while (body_interface.IsActive(sphere_id))
	{
		// Next step
		++step;

		// Output current position and velocity of the sphere
		JPH::RVec3 position = body_interface.GetCenterOfMassPosition(sphere_id);
		JPH::Vec3 velocity = body_interface.GetLinearVelocity(sphere_id);
		cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << endl;

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		// Step the world
		//physics_system.Update(timeManager->getDeltaTime(), cCollisionSteps, &temp_allocator, &job_system);
	}

	// Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
	body_interface.RemoveBody(sphere_id);

	// Destroy the sphere. After this the sphere ID is no longer valid.
	body_interface.DestroyBody(sphere_id);*/
	//
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
