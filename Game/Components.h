#pragma once

/*
Most important role this component serves is to define which entity the player is. When systems see this component, they can define the player functionality.
*/
struct PlayerComponent
{
	int playerNumber;
};
