#include "Header.hlsli"

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	output.Normal = normalize(output.Normal);
    return output;
}