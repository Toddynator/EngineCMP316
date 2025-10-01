#include "pch.h"
#include "Texture.h"
#include "../ResourceLoading/TextureLoader.h"

void Texture::Shutdown()
{
	delete pixels;
	texture->Release();
	textureView->Release();
}