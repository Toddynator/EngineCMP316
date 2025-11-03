/*
Used an enum outside of the shader for light type,
POINT: 0
DIRECTIONAL: 1
SPOT: 2
*/

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct LightBuffer
{
	float4 ambient;
	float4 diffuse;
	float3 position;
	float type;
	float3 attenuation;
	float specularPower;
	float3 direction;
	float range;
	float4 specular;
	float cutoffAngleCosine;
	float3 padding;
};

cbuffer LightsBuffer : register(b0)
{
	int totalActiveLights;
	float3 padding;
	LightBuffer lights[32];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float3 viewVector : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculatePointLight(float3 lightDirection, float3 normal, float4 ldiffuse, float distance, float3 lattenuation, float lrange)
{
	if (distance > lrange)
	{
		return float4(0.f, 0.f, 0.f, 0.f);
	}
	
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(ldiffuse * intensity);
	
	float attenuationFactor = 1.f / (lattenuation.x + (lattenuation.y * distance)) + (lattenuation.z * (distance * distance));
	colour *= attenuationFactor;
	
	float distanceFalloff = 1 - (distance / lrange);
	colour *= distanceFalloff;
	
	return colour;
}

float4 calculateDirectionalLight(float3 lightDirection, float3 normal, float4 ldiffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(ldiffuse * intensity);
	
	return colour;
}

float4 calculateSpotLight(float3 lightToPixel, float3 pixelToLight, float3 lightDirection, float3 normal, float4 ldiffuse, float distance, float3 lattenuation, float lrange, float lcutoffAngleCosine)
{
	/// LIGHT RANGE
	if (distance > lrange)
	{
		return float4(0.f, 0.f, 0.f, 0.f);
	}
	
	/// SPOTLIGHT CONE
	// Light in this case is the centre (lightDirection)
	float cosAngleBetweenLightAndPixel = dot(lightDirection, lightToPixel);

	if (cosAngleBetweenLightAndPixel < lcutoffAngleCosine)
	{
		//return float4(cosAngleBetweenLightAndPixel.xxx, 0.f); // DEBUG
		return float4(0.f.xxxx); 
	}
	
	/// INTENSITY FACTOR
	float intensity = saturate(dot(pixelToLight, normal));
	float4 colour = ldiffuse;
	colour = saturate(colour * intensity);
	
	/// DISTANCE FALLOF
	// Attenuation describes how the intensity of a light diminishes with distance.
	// Thee 3 coefficients mean I can control the intensity of the falloff CURVE.
	// https://gamedev.stackexchange.com/questions/131372/light-attenuation-formula-derivation
	float attenuationFactor = 1.f / (lattenuation.x + (lattenuation.y * distance)) + (lattenuation.z * (distance * distance));
	colour *= attenuationFactor;

	/// ALTERNATIVE / BONUS DISTANCE RANGE FALLOFF
	// This factors in the maximum range as well
	float distanceFalloff = 1 - (distance / lrange);
	colour *= distanceFalloff;
	
	/// CONE CENTRE TO EDGE FALLOFF
	// value of 0 is PURE CENTRE, the cosineCurveRange marks the value value at the very edge 
	// (If it was 90 degrees from the centre, then the value would be 1).
	const float SMOOTHNESS = 0.5f;
	float coneFalloff = (-1) * (1.f - (1.f / (lcutoffAngleCosine / cosAngleBetweenLightAndPixel)));
	coneFalloff = pow(coneFalloff, SMOOTHNESS);
	colour *= coneFalloff; // I conjured up this maths, it works, probably could be made more efficient!
	
	return colour;
}

float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float specularPower)
{
	// blinn-phong specular calculation
	float3 halfway = normalize(lightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfway), 0.0f), specularPower);
	return saturate(specularColour * specularIntensity);
}

float4 main(InputType input) : SV_TARGET
{
	/// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	
	float4 textureColour = texture0.Sample(sampler0, input.tex);
	
	float4 lightColour = float4(0.f, 0.f, 0.f, 0.f);
	float4 specularColour = float4(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < totalActiveLights; i++)
	{
		float3 pixelToLightVector = lights[i].position - input.worldPosition;
		float3 lightToPixelVector = input.worldPosition - lights[i].position; // I like to have both so I can make the distinction clear.
		float distance = length(pixelToLightVector);
		pixelToLightVector = normalize(pixelToLightVector);
		lightToPixelVector = normalize(lightToPixelVector);
		if (lights[i].type == 0)
		{
			/// POINT
			lightColour = lightColour + calculatePointLight(pixelToLightVector, input.normal, lights[i].diffuse, distance, lights[i].attenuation, lights[i].range);
		}
		else if (lights[i].type == 1)
		{
			/// DIRECTIONAL
			lightColour = lightColour + calculateDirectionalLight(lights[i].direction, input.normal, lights[i].diffuse);
		}
		else if (lights[i].type == 2)
		{
			/// SPOTLIGHT
			lightColour = lightColour + calculateSpotLight(lightToPixelVector, pixelToLightVector, lights[i].direction, input.normal, lights[i].diffuse, distance, lights[i].attenuation, lights[i].range, lights[i].cutoffAngleCosine);
		}
		/// SPECULAR
		//specularColour += calcSpecular(pixelToLightVector, input.normal, input.viewVector, lights[i].specular, lights[i].specularPower);
		lightColour += calcSpecular(pixelToLightVector, input.normal, input.viewVector, lights[i].specular, lights[i].specularPower);
        /// AMBIENT
        lightColour += lights[i].ambient;
    }
	
	return (lightColour * textureColour) + specularColour;
}



