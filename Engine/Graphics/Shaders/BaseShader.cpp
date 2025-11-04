#include "pch.h"
#include "BaseShader.h"

bool BaseShader::Initialize(ID3D11Device* rendererDevice, HWND sceneHwnd)
{
	device = rendererDevice;
	hwnd = sceneHwnd;
	// Initialize the vertex and pixel shaders.
	setShaderFilenames();
	if(!initializeShader()) { return false; }
	return true;
}
void BaseShader::Shutdown()
{
	// Release the sampler state.
	if (samplerState)
	{
		samplerState->Release();
		samplerState = nullptr;
	}
	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}
	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}
	// Release the pixel shader.
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}
	// Release the vertex shader.
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
	return;
}

bool BaseShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	/// SAMPLER

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	/// MATRIX BUFFER

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) { return false; }
	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer, 0);
	// Finally set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	return true;
}
void BaseShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexOffset)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(layout);
	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	// Render the mesh.
	deviceContext->DrawIndexed(indexCount, indexOffset, 0);
	return;
}

void BaseShader::setShaderFilenames()
{
	vsFilename = L"data/Shaders/texture_vs.hlsl";
	psFilename = L"data/Shaders/texture_ps.hlsl";
};
bool BaseShader::initializeShader()
{
	loadMatrixBuffer();
	loadSamplerState();
	loadVertexShader();
	loadPixelShader();
	return true;
}
bool BaseShader::loadMatrixBuffer()
{
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if (FAILED(result)) { return false; }
	return true;
}
bool BaseShader::loadSamplerState()
{
	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result)) { return false; }
	return true;
}
bool BaseShader::loadVertexShader()
{
	// Compile the vertex shader code.
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;
	// "main" is the entry point of the shader.
	HRESULT result = D3DCompileFromFile(vsFilename.c_str(), NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, vsFilename.c_str());
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::string vertexShaderFilenameString(vsFilename.begin(), vsFilename.end());
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			std::cout << "\nMissing Vertex Shader File: " << vertexShaderFilenameString;
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(result)) { return false; }

	// Create the vertex input layout description.
	// This setup needs to match the Vertex structure used in the engines meshes and in the shader.
	// D3D11_APPEND_ALIGNED_ELEMENT : This figures out the offset itself based on the previous element, handy!
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	return true;
}
bool BaseShader::loadPixelShader()
{
	// Compile the pixel shader code.
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;
	// "main" is the entry point of the shader, the function that will be called.
	HRESULT result = D3DCompileFromFile(psFilename.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, psFilename.c_str());
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			std::string pixelShaderFilenameString(psFilename.begin(), psFilename.end());
			//MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
			std::cout << "\nMissing Pixel Shader File: " << pixelShaderFilenameString;
		}

		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(result)) { return false; }

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	return true;
}

void BaseShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, const wchar_t* shaderFilename)
{
	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());
	// Get the length of the message.
	unsigned long long bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	std::filesystem::create_directories("logs"); // Creates directory if it doesn't exist
	std::ofstream fout;
	fout.open("logs/shader-error.txt");
	// Write out the error message.
	for (unsigned long long i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = nullptr;

	std::wstring wstr(shaderFilename);
	std::string shaderFilenameString(wstr.begin(), wstr.end());

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	//MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
	std::cout << "\nError compiling shader. Check shader - error.txt for message. File: " << shaderFilenameString;

	return;
}