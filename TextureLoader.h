/*
wrapper for stb image loading library
*/

#pragma once

class TextureLoader
{
public:
	static unsigned char* LoadImage(const char* filename, int* w, int* h, int* channels);
	static void ReleaseImage(unsigned char* data);
};

