/*
wrapper for stb image loading library

unsigned char* is the default image data type, which represents the pixels,
in the future it would be wise to make texture abstract so that I can define texture for different rendering APIs.
*/

#pragma once
#include <d3d11.h>

class TextureLoader
{
public:
	static ID3D11ShaderResourceView* LoadTexture(const char* filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void ReleaseTexture(ID3D11ShaderResourceView* texture);
};

