
/*
DirectX11 Renderer
https://learn.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11
https://learn.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews
*/

#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Renderer_DirectX11
{
public:
    Renderer_DirectX11();
    Renderer_DirectX11(const Renderer_DirectX11& other);
    ~Renderer_DirectX11();

    bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
    void Shutdown();

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    XMMATRIX GetProjectionMatrix() const;
    XMMATRIX GetOrthoMatrix() const;
    void GetVideoCardInfo(char* cardName, int& memory) const;
    D3D11_VIEWPORT GetViewport() { return viewport; }


    void SetBackBufferRenderTarget();
    void ResetViewport();
    void HandleWindowResize(int width, int height, float screenNear, float screenDepth);
    void ToggleWireframe();

private:
    bool initializeDeviceAndSwapChain(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen);
    bool initializeDepthBuffer(int screenWidth, int screenHeight);
    bool initializeDepthStencil();
    bool initializeRasterizer();
    bool initializeViewport(int screenWidth, int screenHeight);
    bool initializeMatrices(int screenWidth, int screenHeight, float screenDepth, float screenNear);

private:
    XMMATRIX projectionMatrix{};
    XMMATRIX orthoMatrix{};

    ID3D11RasterizerState* wireframeRasterizer = nullptr;
    ID3D11RasterizerState* solidFillRasterizer = nullptr;

    bool wireFrameEnabled = false;
    bool vsyncEnabled = false;
    int videoCardMemory = -1;
    char videoCardDescription[128];
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    ID3D11Texture2D* depthStencilBuffer = nullptr;
    ID3D11DepthStencilState* depthStencilState = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;
    D3D11_VIEWPORT viewport{};
};

