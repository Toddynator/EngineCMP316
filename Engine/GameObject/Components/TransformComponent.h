/*
Stores data for transforms in a scene
*/

#pragma once
#include "../Component.h"
#include <directxmath.h>

class TransformComponent : 
	public ClonableComponent<TransformComponent>
{
public:
	/// GETTERS

	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	const DirectX::XMFLOAT3& GetRotation() const { return rotation; }
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	const DirectX::XMFLOAT3& GetOrigin() const { return origin; }
	const DirectX::XMFLOAT3& GetUpVector() const { return upVector; }
	const DirectX::XMFLOAT3& GetForwardVector() const { return forwardVector; }
	const DirectX::XMFLOAT3& GetRightVector() const { return rightVector; }

	// SETTERS (DirectXMath)

	void SetPosition(const DirectX::XMFLOAT3& newPosition) { position = newPosition; }
	void SetRotation(const DirectX::XMFLOAT3& newRotation) { rotation = newRotation; }
	void SetScale(const DirectX::XMFLOAT3& newScale) { scale = newScale; }
	void SetOrigin(const DirectX::XMFLOAT3& newOrigin) { origin = newOrigin; }
	void SetUpVector(const DirectX::XMFLOAT3& newUpVector) { upVector = newUpVector; }
	void SetForwardVector(const DirectX::XMFLOAT3& newForwardVector) { forwardVector = newForwardVector; }
	void SetRightVector(const DirectX::XMFLOAT3& newRightVector) { rightVector = newRightVector; }

	// SETTERS (floats)

	void SetPosition(float newPositionX, float newPositionY, float newPositionZ) { position = { newPositionX, newPositionY, newPositionZ }; }
	void SetRotation(float newRotationX, float newRotationY, float newRotationZ) { rotation = { newRotationX, newRotationY, newRotationZ }; }
	void SetScale(float newScaleX, float newScaleY, float newScaleZ) { scale = { newScaleX, newScaleY, newScaleZ }; }
	void SetOrigin(float newOriginX, float newOriginY, float newOriginZ) { origin = { newOriginX, newOriginY, newOriginZ }; }
	void SetUpVector(float newUpVectorX, float newUpVectorY, float newUpVectorZ) { upVector = { newUpVectorX, newUpVectorY, newUpVectorZ }; }
	void SetForwardVector(float newForwardVectorX, float newForwardVectorY, float newForwardVectorZ) { forwardVector = { newForwardVectorX, newForwardVectorY, newForwardVectorZ }; }
	void SetRightVector(float newRightVectorX, float newRightVectorY, float newRightVectorZ) { rightVector = { newRightVectorX, newRightVectorY, newRightVectorZ }; }

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 origin;
	DirectX::XMFLOAT3 upVector;
	DirectX::XMFLOAT3 forwardVector;
	DirectX::XMFLOAT3 rightVector;
};

