#include "pch.h"
#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture* TextureLoader::LoadTexture(const char* filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Texture* texture = new Texture;

	// stbi_load reads all images as pixels stored as unsigned char.
	// The engine now has to translate this into something the renderer can use.
	int width, height, channels;
	unsigned char* pixels = stbi_load(filepath, &width, &height, &channels, 0);

	// Setup the description of the texture.
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	ID3D11Texture2D* textureResource;
	HRESULT hResult = device->CreateTexture2D(&textureDesc, NULL, &textureResource);
	if (FAILED(hResult))
	{
		std::cout << "\nFailed to create texture from file: ", filepath;
		return NULL;
	}

	// Copy the image data into the texture.
	unsigned int rowPitch = width * channels; // bytes per row
	deviceContext->UpdateSubresource(textureResource, 0, NULL, pixels, rowPitch, 0);

	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	ID3D11ShaderResourceView* textureView;
	hResult = device->CreateShaderResourceView(textureResource, &srvDesc, &textureView);
	if (FAILED(hResult))
	{
		std::cout << "\nFailed to create texture from file: ", filepath;
		return NULL;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(textureView);

	texture->width = width;
	texture->height = height;
	texture->texture = textureResource;
	texture->textureView = textureView;

	return texture;
}

void TextureLoader::ReleaseTexture(Texture* texture)
{
	texture->Shutdown();
}





//unsigned char* TextureLoader::LoadImage(const char* filename, int* w, int* h, int* channels)
//{
//	// stbi_load reads all images as pixels stored as unsigned char.
//	// The engine now has to translate this into something the renderer can use.
//	unsigned char* pixels = stbi_load(filename, w, h, channels, 0);
//	return pixels;
//}
//void TextureLoader::ReleaseImage(unsigned char* data)
//{
//	stbi_image_free(data);
//}
