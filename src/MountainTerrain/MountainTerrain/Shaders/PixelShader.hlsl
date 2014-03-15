#include "Header.hlsli"

float4 main( PS_INPUT input ) : SV_Target
{
	// hard-coding here for now
	float4 ambientColor = {0.05f, 0.05f, 0.05f, 1.0f};
	float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float3 lightDir = {-0.03f, -0.03f, 0.75f};

	// ambient
	float4 color  = ambientColor;

	// diffuse
	lightDir = normalize(-lightDir);
	float lightIntensity = dot(input.Normal, lightDir);
	if(lightIntensity > 0.0f ) {
		color += diffuseColor * lightIntensity;
	}

    return saturate(color);
}