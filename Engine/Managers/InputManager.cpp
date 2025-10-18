#include "pch.h"
#include "InputManager.h"



const CMP316engine::InputManager::KeyBindMap CMP316engine::InputManager::defaultKeybinds
{
	{ "fullscreen", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_F11 }},
	{ "Move Forward", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_W }},
	{ "Move Backward", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_S }},
	{ "Move Up", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_SPACE }},
	{ "Move Down", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_LCTRL }},
	{ "Move Left", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_A }},
	{ "Move Right", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_D }},
	{ "Roll Anti-Clockwise", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_Q }},
	{ "Roll Clockwise", { KeyBinding::KEYBOARD, SDL_Scancode::SDL_SCANCODE_E }}
};



CMP316engine::InputManager::InputManager()
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

bool CMP316engine::InputManager::Initialize()
{
	keybindMap = defaultKeybinds;

	return true;
}

void CMP316engine::InputManager::EndFrame()
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
	SDL_GetMouseState(&mousePositionLastFrame.x, &mousePositionLastFrame.y);
}

void CMP316engine::InputManager::UpdateInputStates(const SDL_Event* event)
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



bool CMP316engine::InputManager::IsKeyBindingPressed(std::string action)
{
	KeyBinding keyBind;
	if (!getKeyBinding(action, keyBind)) { return false; }
	return checkKeyBinding(keyBind.key1, keyBind.keyType1, PRESSED)
		|| checkKeyBinding(keyBind.key2, keyBind.keyType2, PRESSED);
}
bool CMP316engine::InputManager::IsKeyBindingDown(std::string action)
{
	KeyBinding keyBind;
	if (!getKeyBinding(action, keyBind)) { return false; }
	return checkKeyBinding(keyBind.key1, keyBind.keyType1, DOWN)
		|| checkKeyBinding(keyBind.key2, keyBind.keyType2, DOWN);
}
bool CMP316engine::InputManager::IsKeyBindingReleased(std::string action)
{
	KeyBinding keyBind;
	if (!getKeyBinding(action, keyBind)) { return false; }
	return checkKeyBinding(keyBind.key1, keyBind.keyType1, RELEASED)
		|| checkKeyBinding(keyBind.key2, keyBind.keyType2, RELEASED);
}
bool CMP316engine::InputManager::getKeyBinding(std::string action, KeyBinding& keyBind)
{
	auto it = keybindMap.find(action);
	if (it == keybindMap.end()) {
		std::cout << "\nKeybind does not exist: " << action;
		return false;
	}
	keyBind = it->second;
	return true;
}
bool CMP316engine::InputManager::checkKeyBinding(int key, KeyBinding::KeyType keyType, CheckType checkType) const {
	if (keyType == KeyBinding::KEYBOARD)
	{
		auto scancodeKey = static_cast<SDL_Scancode>(key);
		switch (checkType)
		{
		case PRESSED: return IsKeyPressed(scancodeKey);
		case DOWN: return IsKeyDown(scancodeKey);
		case RELEASED: return IsKeyReleased(scancodeKey);
		}
	}
	else if (keyType == KeyBinding::MOUSE)
	{
		auto mouseKey = static_cast<SDL_MouseButtonFlags>(key);
		switch (checkType)
		{
		case PRESSED: return IsMouseButtonPressed(mouseKey);
		case DOWN: return IsMouseButtonDown(mouseKey);
		case RELEASED: return IsMouseButtonReleased(mouseKey);
		}
	}
	return false;
}



bool CMP316engine::InputManager::IsKeyPressed(SDL_Scancode key) const
{
	//if (keys[key] && event->key.scancode && event->type == sdl_event_key_down && !event->key.repeat)
	if (keys[key] && !prevKeys[key]) { return true; }
	return false;
}

bool CMP316engine::InputManager::IsKeyDown(SDL_Scancode key) const
{
	if(keys[key] && prevKeys[key]) { return true; }
	return false;
}

bool CMP316engine::InputManager::IsKeyReleased(SDL_Scancode key) const
{
	if (!keys[key] && prevKeys[key]) { return true; }
	return false;
}

bool CMP316engine::InputManager::IsMouseButtonPressed(SDL_MouseButtonFlags mouseButton) const
{
	if (mouseButtons[mouseButton] && !prevMouseButtons[mouseButton]) { return true; }
	return false;
}

bool CMP316engine::InputManager::IsMouseButtonDown(SDL_MouseButtonFlags mouseButton) const
{
	if (mouseButtons[mouseButton] && prevMouseButtons[mouseButton]) { return true; }
	return false;
}

bool CMP316engine::InputManager::IsMouseButtonReleased(SDL_MouseButtonFlags mouseButton) const
{
	if (!mouseButtons[mouseButton] && prevMouseButtons[mouseButton]) { return true; }
	return false;
}

DirectX::XMFLOAT2 CMP316engine::InputManager::GetMousePositionOnWindow()
{ 
	DirectX::XMFLOAT2 mousePosition;
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y); 
	return mousePosition;
}

float CMP316engine::InputManager::GetMouseDeltaX()
{
	DirectX::XMFLOAT2 currentMousePosition;
	SDL_GetMouseState(&currentMousePosition.x, &currentMousePosition.y);
	mouseDeltaX = mousePositionLastFrame.x - currentMousePosition.x;
	return mouseDeltaX;
}

float CMP316engine::InputManager::GetMouseDeltaY()
{
	DirectX::XMFLOAT2 currentMousePosition;
	SDL_GetMouseState(&currentMousePosition.x, &currentMousePosition.y);
	mouseDeltaY = mousePositionLastFrame.y - currentMousePosition.y;
	return mouseDeltaY;
}

void CMP316engine::InputManager::SetWindowRelativeMouseMode(SDL_Window* window, bool enabled) 
{ 
	SDL_SetWindowRelativeMouseMode(window, enabled); 
}

void CMP316engine::InputManager::SaveCurrentMouseWindowPosition()
{
	SDL_GetMouseState(&mouseSavedPos.x, &mouseSavedPos.y); // Relative to window
}

void CMP316engine::InputManager::SetMouseToSavedPosition(SDL_Window* window)
{
	SDL_WarpMouseInWindow(window, mouseSavedPos.x, mouseSavedPos.y);
}
