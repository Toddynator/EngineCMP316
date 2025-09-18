#include <SDL3/SDL.h>

/*
Provides a clean interface for handling user input from a keyboard and mouse via SDL.
It is designed around keybinds so that users can customize their controls, and support multiple keybinds per action.

SDL Best Practices for Keyboard Input:
https://wiki.libsdl.org/SDL3/BestKeyboardPractices
NOTE:
SDL Uses Scancodes to represent the physical LOCATION of keyboard keys, which are independent of the keyboard layout.
SDL uses SDL_Keycodes to represent the SYMBOL on a key.

HOW TO USE:
- In the applications event poller call updateKeyStates(), it will detect key presses during that frame.
- In the applications main loop call EndFrame() AFTER all inputHandling, this will update the previous keys array which is required for determining if a button is held, just pressed or released.
*/

#pragma once
class InputManager
{
public:
	static const int NUM_MOUSE_BUTTONS = 5;

	bool keys[SDL_SCANCODE_COUNT]; // Array to track the state of each key
	bool prevKeys[SDL_SCANCODE_COUNT]; // Previous Frame
	bool mouseButtons[NUM_MOUSE_BUTTONS]; // SDL_MouseButtonFlags
	bool prevMouseButtons[NUM_MOUSE_BUTTONS];

public:
	InputManager();
	~InputManager() = default;

	void EndFrame();
	void updateInputStates(const SDL_Event* event);

	bool IsKeyPressed(SDL_Scancode key);
	bool IsKeyDown(SDL_Scancode key);
	bool IsKeyReleased(SDL_Scancode key);

	bool IsMouseButtonPressed(SDL_MouseButtonFlags mouseButton);
	bool IsMouseButtonDown(SDL_MouseButtonFlags mouseButton);
	bool IsMouseButtonReleased(SDL_MouseButtonFlags mouseButton);
};

