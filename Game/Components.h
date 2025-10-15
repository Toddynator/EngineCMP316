#pragma once

///// REFLECTION
/*
Basically the exact same as how it is defined in the engines components struct, the Game project has its own Reflector object for initializing the application developers own components.
*/

void InitializeReflection();
static class Reflector
{
public:
	Reflector()
	{
		InitializeReflection();
	}
};
inline static Reflector reflector;

///// COMPONENTS

/*
Most important role this component serves is to define which entity the player is. When systems see this component, they can define the player functionality.
*/
struct PlayerComponent
{
	int playerNumber;
};
