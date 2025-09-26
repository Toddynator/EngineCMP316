
/*
This class defines the viewpoint to draw the 3D scene from.
This class will store the position and rotation of the camera, and generate a view matrix which will then be passed into the HLSL shader for rendering.
*/

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <directxmath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	XMMATRIX GetViewMatrix();

private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix;
};

#endif



