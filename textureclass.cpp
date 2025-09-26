#include "textureclass.h"

TextureClass::TextureClass()
{
	targaData = NULL;
	texture = NULL;
	textureView = NULL;
}

TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// NOTE: Should have a file extension check here in the future to determine what type of texture it is loading.

	// Load the targa image data into memory.
	int bpp = 0;
	result = LoadTarga(filename, bpp);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture.
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
	hResult = device->CreateTexture2D(&textureDesc, NULL, &texture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	if (bpp == 32)
	{
		rowPitch = (width * 4) * sizeof(unsigned char);
	}
	else // bpp == 24
	{
		rowPitch = (width * 3) * sizeof(unsigned char);
	}

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(texture, 0, NULL, targaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(textureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] targaData;
	targaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if (textureView)
	{
		textureView->Release();
		textureView = 0;
	}

	// Release the texture.
	if (texture)
	{
		texture->Release();
		texture = 0;
	}

	// Release the targa data.
	if (targaData)
	{
		delete[] targaData;
		targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return textureView;
}

bool TextureClass::LoadTarga(char* filename, int& bitsPerPixel)
{
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader{};


	// Open the targa file for reading in binary.
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bitsPerPixel;

	bitsPerPixel = bpp;

	if (bpp == 32)
	{
		return LoadTarga32Bit(filename);
	}
	else if (bpp == 24)
	{
		return LoadTarga24Bit(filename);	
	}
	else
	{
		return false;
	}
}

bool TextureClass::LoadTarga32Bit(char* filename)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader{};
	unsigned char* targaImage;


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bitsPerPixel;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	targaData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			targaData[index + 0] = targaImage[k + 2];  // Red.
			targaData[index + 1] = targaImage[k + 1];  // Green.
			targaData[index + 2] = targaImage[k + 0];  // Blue
			targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = 0;

	return true;
}

bool TextureClass::LoadTarga24Bit(char* filename)
{
	/*
	// http://www.paulbourke.net/dataformats/tga/
	*/

	int error, bitsPerPixel, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader{};
	unsigned char* targaImage;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bitsPerPixel = (int)targaFileHeader.bitsPerPixel;

	if (bitsPerPixel != 24)
	{
		return false;
	}

	imageSize = width * height * 3;

	targaImage = new unsigned char[imageSize];

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	targaData = new unsigned char[imageSize];

	index = 0;

	k = (width * height * 3) - (width * 3);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			targaData[index + 0] = targaImage[k + 2];  // Red.
			targaData[index + 1] = targaImage[k + 1];  // Green.
			targaData[index + 2] = targaImage[k + 0];  // Blue

			k += 3;
			index += 3;
		}

		k -= (width * 6);
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}

int TextureClass::GetWidth()
{
	return width;
}


int TextureClass::GetHeight()
{
	return height;
}

