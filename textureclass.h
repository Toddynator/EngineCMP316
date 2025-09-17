/*
Handles loading, unloading and accessing of a single texture resource. Each texture needed requires a new instance of this class.
Uses Targa Files (.tga) as the texture format.

Add more functions here if I want to support other texture formats.
*/

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:
	/// SUPPORTED TEXTURE FORMATS ///
	bool LoadTarga32Bit(char*);

private:
	unsigned char* m_targaData; // Data read in from the targa file.
	ID3D11Texture2D* m_texture; // The structured data that the renderer will use.
	ID3D11ShaderResourceView* m_textureView; // What the shader will use to access the texture when drawing.
	int m_width, m_height;
};

#endif