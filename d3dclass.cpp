#include "d3dclass.h"

#include <string>
#include <iostream>
#include <fstream>

D3DClass::D3DClass()
{
}


D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	// Store the vsync setting.
	vsyncEnabled = vsync;

	/// SWAP CHAIN  + DEVICE CREATION ///
	if (!initializeSwapChain(screenWidth, screenHeight, hwnd, fullscreen)) { return false; }

	/// DEPTH BUFFER ///
	if (!initializeDepthBuffer(screenWidth, screenHeight)) { return false; }
	
	/// DEPTH STENCIL ///
	// Used for Motion blur, volumetric shadows, etc.
	if (!initializeDepthStencil()) { return false; }

	/// RASTERIZER ///
	// Notably Dx11 has a default implementation, but implementing it myself gives more control.
	if (!initializeRasterizer()) { return false; }
	
	/// VIEWPORT ///
	if (!initializeViewport(screenWidth, screenHeight)) { return false; }

	/// MATRICES ///
	// Projection matrix is required for converting the 3d coordinates of the scene into the 2d screen coordinates for rendering.
	// The orthographic matrix is used for 2d rendering, such as for menus and HUDs.
	// Normally you would have a view matrix too, but this is more relevant for a camera system and so it has been left out of this class.
	if (!initializeMatrices(screenWidth, screenHeight, screenDepth, screenNear)) { return false; }
	
	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode otherwise when you release the swap chain it will throw an exception.
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}

	return;
}

// Parameters are a color to clear the screen to at the beginning of each frame.
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	/*
	Helper function, call at the beginning of each frame.
	Initializes buffers to ensure they are blank at the beginning of each frame. (Otherwise you get leftover data / ghosting from the last frame)
	*/

	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	/*
	Helper function, call at the end of each frame.
	It tells the swap chain to display the scene once drawing is complete.
	*/

	// Present the back buffer to the screen since rendering is complete.
	if (vsyncEnabled)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return deviceContext;
}

XMMATRIX D3DClass::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMMATRIX D3DClass::GetOrthoMatrix()
{ 
	return orthoMatrix;
}

// Returns by reference the name and memory of the video card.
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
	return;
}

void D3DClass::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return;
}

void D3DClass::ResetViewport()
{
	// Set the viewport.
	deviceContext->RSSetViewports(1, &viewport);

	return;
}

void D3DClass::HandleWindowResize(int newWidth, int newHeight, float screenNear, float screenDepth)
{
	//https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi#handling-window-resizing

	if (swapChain)
	{
		deviceContext->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		renderTargetView->Release();

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Perform error handling here!

		// Get buffer and create a render-target-view.
		ID3D11Texture2D* pBuffer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)&pBuffer);
		// Perform error handling here!

		hr = device->CreateRenderTargetView(pBuffer, NULL,
			&renderTargetView);
		// Perform error handling here!
		pBuffer->Release();

		deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

		// Set up the viewport.
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(newWidth);
		vp.Height = static_cast<float>(newHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		deviceContext->RSSetViewports(1, &vp);

		/// UPDATE PROJECTION MATRIX

		float screenAspect = static_cast<float>(newWidth) / static_cast<float>(newHeight);
		float fieldOfView = 3.141592654f / 4.0f;

		// Update the projection matrix otherwise the scene will be stretched.
		projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	}
}

bool D3DClass::initializeSwapChain(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen)
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;
	D3D_FEATURE_LEVEL featureLevel;

	// Create a DirectX graphics interface factory.
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	/// SWAP CHAIN ///

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multi-sampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	// If you are on a computer that doesn't have a Dx11 compatible graphics card, then you can swap HARDWARE with 'D3D_DRIVER_TYPE_REFERENCE' to use the CPU instead (Much slower).
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	return true;
}

bool D3DClass::initializeDepthBuffer(int screenWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HRESULT result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool D3DClass::initializeDepthStencil()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HRESULT result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return true;
}

bool D3DClass::initializeRasterizer()
{
	D3D11_RASTERIZER_DESC rasterizerDescription;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT result = device->CreateRasterizerState(&rasterizerDescription, &rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState);

	return true;
}

bool D3DClass::initializeViewport(int screenWidth, int screenHeight)
{
	// Setup the viewport for rendering.
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool D3DClass::initializeMatrices(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	// Create the projection matrix for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	
	return true;
}
