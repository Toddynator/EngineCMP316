#pragma once
#include "Manager.h"
#include <Windows.h>

namespace CMP316engine {
	class WindowManager :
		public Manager
	{
	public:
		virtual bool createWindow() = 0;

		virtual void fullscreenWindow() = 0;

		virtual HWND getHWND() = 0;

	protected:
		bool FULL_SCREEN = false;
	};
}
