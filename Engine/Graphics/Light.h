/*
Represents all Light data structures used by the rendering pipeline
*/

#pragma once
#include <directxmath.h>

enum LightType
{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT
};
struct Light
{
	LightType lightType = LightType::POINT_LIGHT;
	DirectX::XMFLOAT3 lightPosition = { 50.0f, -6.0f, 50.0f };
	DirectX::XMFLOAT3 lightDirection = { 0.f, -1.f, 0.f };
	DirectX::XMFLOAT4 lightDiffuseColour = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 ambienceColour = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 lightSpecularColour = { 0.1f, 0.1f, 0.1f, 0.1f };
	float specularPower = 32.f;
	DirectX::XMFLOAT3 attenuation = { 0.3f, 0.005f, 0.0f };
	float range = 50.f;
	float cutoffAngleRadians = 0.35f; // Spotlight Cone Angle IN RADIANS, should be clamped between 0 to 90 degrees
};