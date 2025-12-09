/*
ABSTRACT CLASS
Acts as an interface for the underlying implementation for window management.

This may be removed in the future, considering I've used SDL for window management, which is already
an abstraction layer. This class is fairly unnecessary.
*/

#pragma once
#include "Manager.h"
#include <Windows.h>
#include <string>

namespace CMP316engine {
	struct WindowProperties
	{
		std::string Title;
		int Width;
		int Height;

		WindowProperties() : Title("CMP316 Engine"), Width(800), Height(600) {}
	};

	class WindowManager :
		public Manager
	{
	public:
		virtual bool Initialize() { return true; }
	
		// Can pass in custom properties for creating window, otherwise it use default values
		virtual bool createWindow(const WindowProperties windowProperties = WindowProperties()) = 0;

		virtual void FullscreenWindow() = 0;
		bool IsFullscreen() { return fullscreen; }
		virtual void GetWindowSize(int& width, int& height) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual HWND GetHWND() const = 0;

	protected:
		bool fullscreen = false;
	};
}
