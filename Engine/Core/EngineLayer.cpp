#include "pch.h"
#include "EngineLayer.h"
#include "../ImGui/imgui_impl_sdl3.h"
#include "../ImGui/imgui_impl_dx11.h"

#include "../Managers/AudioManager_SoLoud.h"
#include "../Managers/WindowManager_SDL.h"
//#include "../Managers/PhysicsManager_Jolt.h"
#include "../ImGui/ImGuizmo/ImGuizmo.h"



CMP316engine::EngineLayer::EngineLayer()
{
	auto& ec = engineContext;
	application = CMP316engine::CreateApp(engineContext);
	ec.timeManager = std::make_unique<TimeManager>();
	ec.inputManager = std::make_unique<InputManager>();
	ec.audioManager = std::make_unique<AudioManager_SoLoud>();
	ec.windowManager = std::make_unique<WindowManager_SDL>();
	ec.physicsManager = std::make_unique<PhysicsManager>();
	ec.sceneManager = std::make_unique<SceneManager>();
	ec.assetManager = std::make_unique<AssetManager>();
}

bool CMP316engine::EngineLayer::Initialize()
{
	auto& ec = engineContext;
	if (!ec.inputManager->Initialize()) { return false; }
	if (!ec.audioManager->Initialize()) { return false; }
	if (!ec.windowManager->Initialize()) { return false; }
	HWND hwnd = ec.windowManager->GetHWND();
	if (!createRenderer(hwnd)) { return false; }
	if (!ec.physicsManager->Initialize()) { return false; }
	if (!ec.sceneManager->Initialize()) { return false; }
	if (!application->Initialize()) { return false; } // Should probably do this last, incase I do any testing with the managers on initialization.
	if (!ec.assetManager->Initialize(ec.renderer->GetDevice(), ec.renderer->GetDeviceContext())) { return false; }

	/////////////
	/// IMGUI ///

	/// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	/// Setup Platform/Renderer back-ends
	ImGui_ImplSDL3_InitForD3D(static_cast<SDL_Window*>(engineContext.windowManager->GetNativeWindow()));
	ImGui_ImplDX11_Init(engineContext.renderer->GetDevice(), engineContext.renderer->GetDeviceContext());

	//////////////////////////////////
	/// ENGINE DEFAULT IMGUI STYLE ///

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 1.f;
	style.FrameBorderSize = 1.f;
	style.TreeLinesFlags = ImGuiTreeNodeFlags_DrawLinesToNodes;
	
	style.Colors[ImGuiCol_FrameBg] = { 70.f / 255.f, 70.f / 255.f, 70.f / 255.f, 150.f / 255.f };
	style.Colors[ImGuiCol_Border] = {  255.f / 255.f, 110.f / 255.f, 128.f / 255.f, 128.f / 255.f };
	style.Colors[ImGuiCol_WindowBg] = { 30.f / 255.f,30.f / 255.f,35.f / 255.f,200.f / 255.f };
	style.Colors[ImGuiCol_TitleBg] = { 20.f / 255.f,20.f / 255.f,30.f / 255.f,255.f / 255.f };
	style.Colors[ImGuiCol_TabSelected] = { 50.f / 255.f,50.f / 255.f, 1.f,1.f };
	style.Colors[ImGuiCol_TabDimmed] = { 20.f / 255.f,20.f / 255.f,50.f / 255.f,1.f };
	style.Colors[ImGuiCol_TabDimmedSelected] = { 35.f / 255.f,70.f / 255.f,180.f / 255.f,1.f };
	style.Colors[ImGuiCol_Tab] = { 55.f / 255.f,50.f / 255.f,120.f / 255.f, 220.f / 255.f };
	style.Colors[ImGuiCol_Button] = {0.f, 110.f/255.f, 1.f, 100.f/255.f};
	style.Colors[ImGuiCol_TreeLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

	///// DEFAULT SHADER
	// TODO: Prob move to assetManager soon

	// Create and initialize the texture shader object.
	engineContext.shader = std::make_unique<Shader>();
	if (!engineContext.shader->Initialize(engineContext.renderer->GetDevice(), hwnd))
	{
		//MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		std::cout << "\nCould not initialize the shader object.";
		return false;
	}

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

	if (engineContext.physicsManager) { engineContext.physicsManager->Shutdown(); }
	if (application) { application->Shutdown(); }
	if (engineContext.shader) { engineContext.shader->Shutdown(); }
	if (engineContext.renderer) { engineContext.renderer->Shutdown(); }
	if (engineContext.windowManager) { engineContext.windowManager->Shutdown(); }
	if (engineContext.audioManager) { engineContext.audioManager->Shutdown(); }
	if (engineContext.inputManager) { engineContext.inputManager->Shutdown(); }
	if (engineContext.sceneManager) { engineContext.sceneManager->Shutdown(); }
	if (engineContext.assetManager) { engineContext.assetManager->Shutdown(); }
}

bool CMP316engine::EngineLayer::processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0)
	{
		////////////////////
		/// WINDOW CLOSE ///

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

		//////////////////////
		/// MANAGERS / API ///

		ImGui_ImplSDL3_ProcessEvent(&event);
		engineContext.inputManager->UpdateInputStates(&event);
	}
	return true;
}

void CMP316engine::EngineLayer::update()
{
	///// IMGUI  

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0U, (const ImGuiViewport*)0, ImGuiDockNodeFlags_PassthruCentralNode); // DOCKING: Supports docking windows to the viewport, must be rendered before other ImGui Windows
	ImGuizmo::BeginFrame(); // IMGUIZMO

	///// MANAGERS PRE-APPLICATION UPDATE

	engineContext.timeManager->Update();
	float deltaTime = engineContext.timeManager->getDeltaTime();
	engineContext.sceneManager->Update(engineContext);

	///// APPLICATION

	application->HandleInput(deltaTime);
	application->HandleImGui();
	application->Update(deltaTime);

	///// MANAGERS POST-APPLICATION UPDATE

	engineContext.physicsManager->Update(deltaTime);
	engineContext.inputManager->EndFrame();
}

void CMP316engine::EngineLayer::render()
{
	engineContext.renderer->BeginScene(0.05f, 0.08f, 0.1f, 1.0f); // Clear Colour
	///// APPLICATION
	application->Render();
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
		//MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		std::cout << "\nCould not initialize Direct3D";
		return false;
	}

	return true;
}
