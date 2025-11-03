#pragma once
#include "BaseShader.h"
class TextureShader : public BaseShader
{
protected:
	virtual void setShaderFilenames() override
	{
		vsFilename = L"data/Shaders/texture_vs.hlsl";
		psFilename = L"data/Shaders/texture_ps.hlsl";
	}
};

