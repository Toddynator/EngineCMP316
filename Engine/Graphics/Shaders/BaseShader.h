/*
ABSTRACT BASE CLASS
Initially based off Rastertek Tutorial on DirectX11.
https://rastertek.com/index.html

Derived Classes should at minimum be defining the shader filenames.
*/

#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
class BaseShader
{
public:
	BaseShader() {}
	~BaseShader() { Shutdown(); }

	bool Initialize(ID3D11Device* device, HWND hwnd);
	virtual void Shutdown();

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	// Index Offset applies if a buffer was shared between meshes
	void Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexOffset = 0);

protected:
	virtual void setShaderFilenames() = 0;
	// OVERRIDE and call base implementation to add additional buffers
	// @note Support for Additional Shaders should be defined by this class
	virtual bool initializeShader();
	bool loadMatrixBuffer();
	bool loadSamplerState();
	bool loadVertexShader();
	bool loadPixelShader();

private:
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, const wchar_t* shaderFilename);

protected:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	HWND hwnd;
	ID3D11Device* device = nullptr;
	std::wstring vsFilename;
	std::wstring psFilename;

private:
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};