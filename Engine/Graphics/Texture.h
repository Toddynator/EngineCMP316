/*
Definition of a DX11 Texture
TODO:
- Possibly make this just store the pixels and rename to 'RawTexture'.
I could then have Render specific textures handled by the renderer itself or implementations of the AssetManager (e.g. AssetManager_DirectX11)
*/

#pragma once
#include <d3d11.h>

class Texture
{
	friend class TextureLoader; // Let textureLoader set the Texture up
public:
	Texture() = default;
	~Texture() = default;

	void Shutdown();

	ID3D11ShaderResourceView* GetTextureView() { return textureView; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

private:
	int width = 0;
	int height = 0;
	unsigned char* pixels = nullptr; // The raw data from the loaded texture
	ID3D11Texture2D* texture = nullptr; // The structured data that the renderer will use.
	ID3D11ShaderResourceView* textureView = nullptr; // What the shader will use to access the texture when drawing.
};