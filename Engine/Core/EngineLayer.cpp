#include "pch.h"
#include "EngineLayer.h"
#include "../ImGui/imgui_impl_sdl3.h"
#include "../ImGui/imgui_impl_dx11.h"

#include "../Managers/AudioManager_SoLoud.h"
#include "../Managers/WindowManager_SDL.h"



CMP316engine::EngineLayer::EngineLayer()
{
	auto& ec = engineContext;
	application = CMP316engine::CreateApp(engineContext);
	//application = CMP316engine::CreateApp();
	ec.timeManager = std::make_unique<TimeManager>();
	//ec.inputManager = std::make_unique<InputManager>();
	ec.audioManager = std::make_unique<AudioManager_SoLoud>();
	ec.windowManager = std::make_unique<WindowManager_SDL>();
	//ec.physicsManager = std::make_unique<PhysicsManager>();
}

bool CMP316engine::EngineLayer::Initialize()
{
	auto& ec = engineContext;
	//if (!ec.inputManager->Initialize()) { return false; }
	if (!ec.audioManager->Initialize()) { return false; }
	if (!ec.windowManager->Initialize()) { return false; }
	HWND hwnd = ec.windowManager->GetHWND();
	if (!createRenderer(hwnd)) { return false; }
	//if (!ec.physicsManager->Initialize()) { return false; }
	if (!application->Initialize()) { return false; } // Should probably do this last, incase I do any testing with the managers on initialization.

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
	ImGui_ImplSDL3_InitForD3D(static_cast<SDL_Window*>(engineContext.windowManager->GetNativeWindow()));
	ImGui_ImplDX11_Init(engineContext.renderer->GetDevice(), engineContext.renderer->GetDeviceContext());

	/////////////
	/// SCENE ///

	// Create and initialize the texture shader object.
	engineContext.shader = std::make_unique<Shader>();

	if (!engineContext.shader->Initialize(engineContext.renderer->GetDevice(), hwnd))
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

	if (!model->Initialize(engineContext.renderer->GetDevice(), engineContext.renderer->GetDeviceContext()))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/// PHYSICS TEST ///
	// We'll just associate this with our model for now
	//JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
	//modelPhysicsBodyID = engineContext.physicsManager->GetBodyInterface().CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	//engineContext.physicsManager->GetBodyInterface().SetLinearVelocity(modelPhysicsBodyID, JPH::Vec3(0.1f, 0.1f, 0.0f));
	//
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	///


	return true;
}

void CMP316engine::EngineLayer::Run()
{
	while (true)
	{
		if (!processEvents()) { return; }
		update();
		render();
	}
}

void CMP316engine::EngineLayer::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	//if (engineContext.physicsManager) { engineContext.physicsManager->Shutdown(); }
	if (application) { application->Shutdown(); }
	if (engineContext.shader) { engineContext.shader->Shutdown(); }
	if (model) { model->Shutdown(); }
	if (engineContext.renderer) { engineContext.renderer->Shutdown(); }
	if (engineContext.windowManager) { engineContext.windowManager->Shutdown(); }
	if (engineContext.audioManager) { engineContext.audioManager->Shutdown(); }
}

bool CMP316engine::EngineLayer::processEvents()
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
			engineContext.windowManager->GetWindowSize(screenWidth, screenHeight);
			engineContext.renderer->HandleWindowResize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
		}

		/////////////
		/// INPUT ///

		ImGui_ImplSDL3_ProcessEvent(&event);
		//engineContext.inputManager->UpdateInputStates(&event);
	}
	return true;
}

void CMP316engine::EngineLayer::update()
{
	///// TIME

	engineContext.timeManager->Update();

	///// INPUT

	// TODO: Make a global inputs function for encapsulating application input
	//if (engineContext.inputManager->IsKeyBindingPressed("fullscreen")) {
	//	engineContext.windowManager->FullscreenWindow();
	//}
	//engineContext.inputManager->EndFrame(); // Should move this to the very end, just in case maybe the update loop for whatever reason has input calls for example.

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
		engineContext.renderer->ToggleWireframe();
	}
	model->RenderImGuiControls();
	ImGui::End();
	// TEST

	///// SCENE

	application->HandleInput();
	application->HandleImgui();
	application->Update(engineContext.timeManager->getDeltaTime());

	/// PHYSICS TEST

	//engineContext.physicsManager->Update(engineContext.timeManager->getDeltaTime());
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	//model->SetPosition(XMFLOAT3(position.GetX(), position.GetY(), position.GetZ()));

	///
}

void CMP316engine::EngineLayer::render()
{
	XMMATRIX viewMatrix, projectionMatrix;
	engineContext.renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f); // Black

	///// SCENE

	application->Render();

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the view, and projection matrices from the camera and d3d objects.
	viewMatrix = camera->GetViewMatrix();
	projectionMatrix = engineContext.renderer->GetProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// CALL THIS FOR EACH RENDERABLE OBJECT IN THE SCENE
	model->Render(engineContext.shader.get(), engineContext.renderer->GetDeviceContext(), viewMatrix, projectionMatrix);
	
	///// IMGUI

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	/////

	engineContext.renderer->EndScene();
}

bool CMP316engine::EngineLayer::createRenderer(HWND hwnd)
{
	engineContext.renderer = std::make_unique<Renderer_DirectX11>();

	int screenWidth, screenHeight;
	screenWidth = 0; screenHeight = 0;
	engineContext.windowManager->GetWindowSize(screenWidth, screenHeight);

	if (!engineContext.renderer->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, engineContext.windowManager->IsFullscreen(), SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}
