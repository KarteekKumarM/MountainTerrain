cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
	float3 Normal : NORMAL0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------               
VS_OUTPUT VS( float4 Position : POSITION, float4 Color : COLOR, float3 Normal : NORMAL )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Color = Color;

	output.Normal = normalize(Normal);
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	// hard-coding here from now
	float4 ambientColor = {0.01f, 0.01f, 0.01f, 1.0f};
	//float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float3 lightDir = {1.0f, 1.0f, 1.0f};
	float lightIntensity = 1.0f;
	
	float4 color  = ambientColor;

	// reverse the vector
	lightDir = -lightDir;

	lightIntensity = saturate( dot(input.Normal, lightDir) );
	if(lightIntensity > 0 ) {
		color += input.Color * lightIntensity;
	}

	color = saturate(color);

    return color;
}