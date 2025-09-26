
/*
DirectX11 Renderer
*/

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class D3DClass
{
public:
    D3DClass();
    D3DClass(const D3DClass& other);
    ~D3DClass();

    bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
    void Shutdown();

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    XMMATRIX GetProjectionMatrix();
    XMMATRIX GetOrthoMatrix();
    void GetVideoCardInfo(char* cardName, int& memory);

    void SetBackBufferRenderTarget();
    void ResetViewport();
    void HandleWindowResize(int width, int height, float screenNear, float screenDepth);

private:
    bool initializeSwapChain(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen);
    bool initializeDepthBuffer(int screenWidth, int screenHeight);
    bool initializeDepthStencil();
    bool initializeRasterizer();
    bool initializeViewport(int screenWidth, int screenHeight);
    bool initializeMatrices(int screenWidth, int screenHeight, float screenDepth, float screenNear);

private:
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;

    bool vsyncEnabled = false;
    int videoCardMemory;
    char videoCardDescription[128];
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    ID3D11Texture2D* depthStencilBuffer = nullptr;
    ID3D11DepthStencilState* depthStencilState = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;
    ID3D11RasterizerState* rasterState = nullptr;
    D3D11_VIEWPORT viewport;
};

#endif

