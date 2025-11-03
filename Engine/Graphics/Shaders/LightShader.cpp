#include "pch.h"
#include "LightShader.h"

void LightShader::Shutdown()
{
	if (lightsBuffer)
	{
		lightsBuffer->Release();
		lightsBuffer = nullptr;
	}
	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}
	BaseShader::Shutdown();
}

void LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 cameraPosition, std::vector<Light> lights)
{
	BaseShader::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);

	/// LIGHTS

	LightsBufferType* lightsPtr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(lightsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightsPtr = (LightsBufferType*)mappedResource.pData;
	int lightArraySize = std::size(lightsPtr->lights);
	if (lights.size() > lightArraySize) { std::cout << "\nLight Vector greater than programs max number of lights"; }
	for (int i = 0; i < lightArraySize; i++)
	{		
		if (i >= lights.size()) { break; }
		auto& light = lights[i];

		lightsPtr->lights[i].ambient = light.ambienceColour;
		lightsPtr->lights[i].diffuse = light.lightDiffuseColour;
		lightsPtr->lights[i].position = light.lightPosition;
		lightsPtr->lights[i].type = static_cast<float>(light.lightType);
		lightsPtr->lights[i].attenuation = light.attenuation;
		lightsPtr->lights[i].specularPower = light.specularPower;
		lightsPtr->lights[i].direction = light.lightDirection;
		lightsPtr->lights[i].range = light.range;
		lightsPtr->lights[i].specularColour = light.lightSpecularColour;
		lightsPtr->lights[i].cutoffAngleCosine = std::cos(light.cutoffAngleRadians);
		lightsPtr->lights[i].padding = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}
	lightsPtr->totalActiveLights = static_cast<int>(lights.size());
	lightsPtr->padding = { 0.f,0.f,0.f };
	deviceContext->Unmap(lightsBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightsBuffer);

	/// CAMERA

	CameraBufferType* cameraPtr;
	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->cameraPosition = cameraPosition;
	cameraPtr->padding = 0.0f;
	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &cameraBuffer);
}

bool LightShader::initializeShader()
{
	BaseShader::initializeShader();

	/// LIGHTS

	D3D11_BUFFER_DESC lightsBufferDesc;
	lightsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightsBufferDesc.ByteWidth = sizeof(LightsBufferType);
	lightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightsBufferDesc.MiscFlags = 0;
	lightsBufferDesc.StructureByteStride = 0;
	device->CreateBuffer(&lightsBufferDesc, NULL, &lightsBuffer);

	/// CAMERA

	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	device->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

	return true;
}