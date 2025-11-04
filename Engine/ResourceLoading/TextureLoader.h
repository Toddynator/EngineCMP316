/*
wrapper for stb image loading library

unsigned char* is the default image data type, which represents the pixels,
in the future it would be wise to make texture abstract so that I can define texture for different rendering APIs.
*/

#pragma once
#include <d3d11.h>
#include "Graphics/Texture.h"
class TextureLoader
{
public:
	// @brief Will load texture file as pixels then create the Texture that the renderer can use.
	static Texture* LoadTexture(const char* filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	// @brief Creates a renderer compatible Texture, this can be used to create Textures at runtime.
	static Texture* CreateRendererTexture(unsigned char* pixels, int width, int height, int channels, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void ReleaseTexture(Texture* texture);
};

