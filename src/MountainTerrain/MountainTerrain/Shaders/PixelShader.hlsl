#include "Header.hlsli"

Texture2D g_grassTexture : register(t0);
Texture2D g_rockTexture : register(t1);
SamplerState g_Sampler;


float4 main( PS_INPUT input ) : SV_Target
{
	// hard-coding here for now
	float4 ambientColor = { 0.05f, 0.05f, 0.05f, 1.0f };
	float3 lightDir = { -0.03f, -0.03f, 0.75f };
	float3 upDir = { 0.0f, 1.0f, 0.0f };

	float2 texCoOrd = { 0.0f, 0.0f };

	// TO DO : Fix, so texture does not stretch with height
	texCoOrd[0] = (input.TextureCoordinates.x);
	texCoOrd[1] = (input.TextureCoordinates.z);

	// invert light direction 
	lightDir = normalize(-lightDir);


	// calculate texture color
	float4 textureColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	float slopeFactor = abs(dot(input.Normal, upDir));
	/*if (slopeFactor < 0.6)
	{
		textureColor = g_rockTexture.Sample(g_Sampler, texCoOrd);
	}
	else
	{
		textureColor = g_grassTexture.Sample(g_Sampler, texCoOrd);
	}*/
	textureColor = (slopeFactor*g_grassTexture.Sample(g_Sampler, texCoOrd)) + ((1 - slopeFactor)*g_rockTexture.Sample(g_Sampler, texCoOrd));

	// ambient
	float4 color  = ambientColor;

	// diffuse
	float lightIntensity = dot(input.Normal, lightDir);
	if(lightIntensity > 0.0f ) {
		color += textureColor * lightIntensity;
	}

    return saturate(color);
}