
/*
Provides a clean interface for handling user input from a keyboard and mouse via SDL.
It is designed around keybinds so that users can customize their controls, and support multiple keybinds per action.

SDL MOUSE:
https://wiki.libsdl.org/SDL3/CategoryMouse

SDL Best Practices for Keyboard Input:
https://wiki.libsdl.org/SDL3/BestKeyboardPractices
NOTE:
SDL Uses Scancodes to represent the physical LOCATION of keyboard keys, which are independent of the keyboard layout.
SDL uses SDL_Keycodes to represent the SYMBOL on a key.

HOW TO USE:
- In the applications event poller call UpdateKeyStates(), it will detect key presses during that frame.
- In the applications main loop call EndFrame() AFTER all inputHandling, this will update the previous keys array which is required for determining if a button is held, just pressed or released.

- Recommended all input checks should use Key binds instead of directly checking a particular type of key. Key binds support remapping instead of hard coded inputs.
This also allows for better support on other platforms, of which I can then have the default keybind map change to the desired platform.
*/

#pragma once
#include "Manager.h"
#include <SDL3/SDL.h>
#include <functional>
#include <DirectXMath.h>

namespace CMP316engine {
	struct KeyBindingKey
	{
		enum KeyType
		{
			NONE,
			KEYBOARD,
			MOUSE
		};

		int key = 0;
		KeyType keyType = KeyType::NONE;

		KeyBindingKey(KeyType newType, int newKey) : keyType(newType), key(newKey) {}
	};

	/*
	Supports multiple keys for one key bind / action,
	and each keybindg key can support multiple keys for the binding, e.g. Ctrl + C for copying, with for example a second key being 'k + left mouse + l' (any combination of keys).
	*/
	struct KeyBinding
	{
		std::vector<KeyBindingKey> keyCombo1;
		std::vector<KeyBindingKey> keyCombo2;

		// Second keyCombo is optional
		KeyBinding(std::vector<KeyBindingKey> newKeyCombo1, std::vector<KeyBindingKey> newKeyCombo2 = {}) : keyCombo1(newKeyCombo1), keyCombo2(newKeyCombo2) {}
		KeyBinding() = default;
	};

	class InputManager :
		public Manager
	{
	private:
		using KeyBindMap = std::unordered_map<std::string, KeyBinding>;
		KeyBindMap keybindMap;
		static const KeyBindMap defaultKeybinds; // Allows returning to default settings at a later date.

		bool keys[SDL_SCANCODE_COUNT]; // Array to track the state of each key
		bool prevKeys[SDL_SCANCODE_COUNT]; // Previous Frame
		static const int NUM_MOUSE_BUTTONS = 5;
		bool mouseButtons[NUM_MOUSE_BUTTONS]; // SDL_MouseButtonFlags
		bool prevMouseButtons[NUM_MOUSE_BUTTONS];

		DirectX::XMFLOAT2 mousePositionLastFrame;
		DirectX::XMFLOAT2 mouseSavedPos;
		float mouseDeltaX;
		float mouseDeltaY;

	public:
		InputManager();
		~InputManager() = default;

		bool Initialize();
		void Shutdown() override {}

		void EndFrame();
		void UpdateInputStates(const SDL_Event* event);

		bool IsKeyBindingPressed(std::string action);
		bool IsKeyBindingDown(std::string action);
		bool IsKeyBindingReleased(std::string action);

		bool IsKeyPressed(SDL_Scancode key) const;
		bool IsKeyDown(SDL_Scancode key) const;
		bool IsKeyReleased(SDL_Scancode key) const;

		bool IsMouseButtonPressed(SDL_MouseButtonFlags mouseButton) const;
		bool IsMouseButtonDown(SDL_MouseButtonFlags mouseButton) const;
		bool IsMouseButtonReleased(SDL_MouseButtonFlags mouseButton) const;

		DirectX::XMFLOAT2 GetMousePositionOnWindow();
		float GetMouseDeltaX();
		float GetMouseDeltaY();

		// Hides cursor and constrains the mouse to the window but keeps reading mouse delta even when mouse has hit window edge.
		void SetWindowRelativeMouseMode(SDL_Window* window, bool enabled);
		void SaveCurrentMouseWindowPosition();
		void SetMouseToSavedPosition(SDL_Window* window);

	private:
		enum CheckType
		{
			PRESSED,
			DOWN,
			RELEASED
		};
		bool getKeyBinding(std::string action, KeyBinding& keyBind);
		// Checks if keyBinding key is pressed. Encapsulates choosing the correct function based on the keys type.
		bool checkKeyBinding(std::vector<KeyBindingKey> keyBindingCombo, CheckType checkType) const;
	};
}

