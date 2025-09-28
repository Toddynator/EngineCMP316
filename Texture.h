/*
Handles loading, unloading and accessing of a single texture resource. Each texture needed requires a new instance of this class.
Uses Targa Files (.tga) as the texture format.

Add more functions here if I want to support other texture formats.

NOTE:
- Targa Loading functions are due a refactoring, currently some unnecessary code duplication.
*/

#pragma once
#include <d3d11.h>

class Texture
{
	friend class TextureLoader; // Let textureLoader set the Texture up
public:
	Texture() {};
	~Texture() = default;

	void Shutdown();

	ID3D11ShaderResourceView* GetTextureView() { return textureView; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	int width;
	int height;
	unsigned char* pixels; // The raw data from the loaded texture
	ID3D11Texture2D* texture; // The structured data that the renderer will use.
	ID3D11ShaderResourceView* textureView; // What the shader will use to access the texture when drawing.
};