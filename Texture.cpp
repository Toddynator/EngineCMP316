#include "Texture.h"
#include "TextureLoader.h"

void Texture::Shutdown()
{
	delete pixels;
	texture->Release();
	textureView->Release();
}