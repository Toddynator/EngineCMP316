#pragma once
#include "BaseShader.h"
#include "Graphics/Light.h"

static constexpr int NUM_LIGHTS = 32;

class LightShader : public BaseShader
{
private:
	struct alignas(16) LightBufferType
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT3 position;
		float type;
		DirectX::XMFLOAT3 attenuation;
		float specularPower;
		DirectX::XMFLOAT3 direction;
		float range;
		DirectX::XMFLOAT4 specularColour;
		float cutoffAngleCosine;
		DirectX::XMFLOAT3 padding;
	};
	/*
	TODO: Heard there may be a 'StructuredBuffer' that I can use instead which means I would no longer need a FIXED light array!
	*/
	struct alignas(16) LightsBufferType
	{
		int totalActiveLights; // Actual number of lights being used currently
		DirectX::XMFLOAT3 padding;
		LightBufferType lights[NUM_LIGHTS]; // The container for scene lights, has a fixed number that can't be exceeded during runtime. NOTE: If I change the size, remember to change in the Pixel Shader!
	};
	struct alignas(16) CameraBufferType
	{
		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};

public:
	virtual void Shutdown() override;
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 cameraPosition, std::vector<Light> lights);

protected:
	virtual bool initializeShader() override;
	virtual void setShaderFilenames() override
	{
		vsFilename = L"data/Shaders/light_vs.hlsl";
		psFilename = L"data/Shaders/light_ps.hlsl";
	}

private:
	ID3D11Buffer* lightsBuffer;
	ID3D11Buffer* cameraBuffer;
};

