#include "pch.h"
#include "InputManager.h"



const CMP316engine::InputManager::KeyBindMap CMP316engine::InputManager::defaultKeybinds
{
	{ "fullscreen", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, static_cast<int>(SDL_SCANCODE_F11)) }) },
	{ "Move Forward", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_W) }) },
	{ "Move Backward", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_S) }) },
	{ "Move Up", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_SPACE) }) },
	{ "Move Down", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_LCTRL) }) },
	{ "Move Left", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_A) }) },
	{ "Move Right", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_D) }) },
	{ "Roll Anti-Clockwise", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_Q) }) },
	{ "Roll Clockwise", KeyBinding({ KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_E) }) },

	{ "Copy", KeyBinding({
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_LCTRL),
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_C)
	}) },

	{ "Cut", KeyBinding({
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_LCTRL),
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_X)
	}) },

	{ "Paste", KeyBinding({
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_LCTRL),
		KeyBindingKey(KeyBindingKey::KEYBOARD, SDL_SCANCODE_V)
	}) }
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
	return checkKeyBinding(keyBind.keyCombo1, PRESSED)
		|| checkKeyBinding(keyBind.keyCombo2, PRESSED);
}
bool CMP316engine::InputManager::IsKeyBindingDown(std::string action)
{
	KeyBinding keyBind;
	if (!getKeyBinding(action, keyBind)) { return false; }
	return checkKeyBinding(keyBind.keyCombo1, DOWN)
		|| checkKeyBinding(keyBind.keyCombo2, DOWN);
}
bool CMP316engine::InputManager::IsKeyBindingReleased(std::string action)
{
	KeyBinding keyBind;
	if (!getKeyBinding(action, keyBind)) { return false; }
	return checkKeyBinding(keyBind.keyCombo1, RELEASED)
		|| checkKeyBinding(keyBind.keyCombo2, RELEASED);
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
bool CMP316engine::InputManager::checkKeyBinding(std::vector<KeyBindingKey> keyBindingCombo, CheckType checkType) const {
	/*
	In order to handle multi-key-combos, as users will realistically never be able to press both keys at the exact same frame, only the last key in the combo needs
	to obey the checkType.
	Every key before the last in the combo is treated as HELD.
	*/

	if (keyBindingCombo.empty()) { return false; }
	CheckType currentCheckType;
	int keyNum = 0;
	for (auto& keyBindingKey : keyBindingCombo)
	{
		if (keyNum < keyBindingCombo.size() - 1) { currentCheckType = CheckType::DOWN; }
		else { currentCheckType = checkType; }

		if (keyBindingKey.keyType == KeyBindingKey::KEYBOARD)
		{
			auto scancodeKey = static_cast<SDL_Scancode>(keyBindingKey.key);
			switch (currentCheckType)
			{
			case PRESSED: if (!IsKeyPressed(scancodeKey)) { return false; }; break;
			case DOWN: if (!IsKeyDown(scancodeKey)) { return false; };  break;
			case RELEASED: if (!IsKeyReleased(scancodeKey)) { return false; };  break;
			}
		}
		else if (keyBindingKey.keyType == KeyBindingKey::MOUSE)
		{
			auto mouseKey = static_cast<SDL_MouseButtonFlags>(keyBindingKey.key);
			switch (currentCheckType)
			{
			case PRESSED: if (!IsMouseButtonPressed(mouseKey)) { return false; };  break;
			case DOWN: if (!IsMouseButtonDown(mouseKey)) { return false; };  break;
			case RELEASED: if (!IsMouseButtonReleased(mouseKey)) { return false; };  break;
			}
		}

		keyNum++;
	}
	return true;
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
