#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
	// Initialize all keys to not pressed
	for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) 
	{
		keys[i] = false;
		prevKeys[i] = false;
	}
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		mouseButtons[i] = false;
		prevMouseButtons[i] = false;
	}
}

void InputManager::EndFrame()
{
	// Keyboard
	for (int key = 0; key < SDL_SCANCODE_COUNT; key++)
	{
		prevKeys[key] = keys[key];
	}
	// Mouse
	for (int button = 0; button < NUM_MOUSE_BUTTONS; button++)
	{
		prevMouseButtons[button] = mouseButtons[button];
	}
}

void InputManager::updateInputStates(const SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_EVENT_KEY_DOWN:
		keys[event->key.scancode] = true;
		break;
	case SDL_EVENT_KEY_UP:
		keys[event->key.scancode] = false;
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseButtons[event->button.button] = true;
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouseButtons[event->button.button] = false;
		break;
	default:
		break;
	}
}

bool InputManager::IsKeyPressed(SDL_Scancode key)
{
	//if (keys[key] && event->key.scancode && event->type == sdl_event_key_down && !event->key.repeat)
	if (keys[key] && !prevKeys[key]) { return true; }
	return false;
}

bool InputManager::IsKeyDown(SDL_Scancode key)
{
	if(keys[key] && prevKeys[key]) { return true; }
	return false;
}

bool InputManager::IsKeyReleased(SDL_Scancode key)
{
	if (!keys[key] && prevKeys[key]) { return true; }
	return false;
}

bool InputManager::IsMouseButtonPressed(SDL_MouseButtonFlags mouseButton)
{
	if (mouseButtons[mouseButton] && !prevMouseButtons[mouseButton]) { return true; }
	return false;
}

bool InputManager::IsMouseButtonDown(SDL_MouseButtonFlags mouseButton)
{
	if (mouseButtons[mouseButton] && prevMouseButtons[mouseButton]) { return true; }
	return false;
}

bool InputManager::IsMouseButtonReleased(SDL_MouseButtonFlags mouseButton)
{
	if (!mouseButtons[mouseButton] && prevMouseButtons[mouseButton]) { return true; }
	return false;
}
