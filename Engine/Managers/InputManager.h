
/*
Provides a clean interface for handling user input from a keyboard and mouse via SDL.
It is designed around keybinds so that users can customize their controls, and support multiple keybinds per action.

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

namespace CMP316engine {
	/*
	Supports multiple keys for one key bind / action
	*/
	struct KeyBinding
	{
		enum KeyType
		{
			NONE,
			KEYBOARD,
			MOUSE
		};

		int key1 = 0;
		int key2 = 0;
		KeyType keyType1 = NONE;
		KeyType keyType2 = NONE;

		KeyBinding(KeyType newKeyType1, int newKey1, KeyType newKeyType2 = NONE, int newKey2 = 0) : keyType1(newKeyType1), key1(newKey1), keyType2(newKeyType2), key2(newKey2) {}
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

	public:
		InputManager();
		~InputManager() = default;

		bool Initialize() override;
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

	private:
		enum CheckType
		{
			PRESSED,
			DOWN,
			RELEASED
		};
		bool getKeyBinding(std::string action, KeyBinding& keyBind);
		// Checks if keyBinding key is pressed. Encapsulates choosing the correct function based on the keys type.
		bool checkKeyBinding(int key, KeyBinding::KeyType keyType, CheckType checkType) const;
	};
}

