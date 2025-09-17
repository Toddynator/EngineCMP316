/*
PIXEL SHADER
Written in HLSL (High Level Shader Language)

Draws each pixel on the polygons that are rendered to the screen.
GETS ITS INPUT FROM THE VERTEX SHADER
*/

/// TYPEDEFS ///

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

/// PIXEL SHADER ///

// Returns the final calculated colour of the pixel
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
    return input.color;
    //return input.color * 0.5f; // Makes it half as bright
}