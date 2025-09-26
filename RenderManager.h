/*
Structure based off d3dclass, may change in the future
need to figure out how to abstract d3d from other graphic related classes such as the shaders, etc.
*/

#pragma once
#include "Manager.h"
#include "Windows.h"

namespace CMP316engine {
	class RenderManager
		: public Manager
	{
		bool createRenderer(HWND hwnd);

		virtual void BeginScene(float red, float green, float blue, float alpha) = 0; // Clears scene to colour set in parameters
		virtual void EndScene() = 0; // Display Scene

		virtual void HandleWindowResize(int width, int height, float screenNear, float screenDepth) = 0;

		virtual void* GetDevice() = 0;
		virtual void* GetDeviceContext() = 0;
		virtual void* GetProjectionMatrix() = 0;
		virtual void* GetOrthoMatrix() = 0;
	};
}
