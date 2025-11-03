Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 colour : COLOR;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 finalColour;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColour = shaderTexture.Sample(SampleType, input.tex);
	finalColour = textureColour * input.colour;
	
	return finalColour;
}