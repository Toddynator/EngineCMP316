#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* TextureLoader::LoadImage(const char* filename, int* w, int* h, int* channels)
{
	return stbi_load(filename, w, h, channels, 0);
}

void TextureLoader::ReleaseImage(unsigned char* data)
{
	stbi_image_free(data);
}
