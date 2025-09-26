/*
Where levels, aka scenes will go inside an application.
Scenes will handle the 'Scene Tree' where all GameObjects are stored.

Current plan is that games will simply override this and create their own definitions of levels.
However,
may consider having levels be entirely serialized, and created through an engine level editor, in which case you would only need one scene class for all levels in a game.
*/

#pragma once

namespace CMP316engine {
	class Scene
	{
	public:
		virtual void HandleInput();
		virtual void HandleImgui();
		virtual void Update(float deltaTime);
		virtual void Render();
	};
}

