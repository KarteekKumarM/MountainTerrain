cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT
{
	float4 Position: POSITION; 
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------               
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	output.Normal = normalize(output.Normal);
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	// hard-coding here for now
	float4 ambientColor = {0.05f, 0.05f, 0.05f, 1.0f};
	float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float3 lightDir = {-0.03f, -0.03f, 0.75f};

	// ambient
	float4 color  = ambientColor;

	// diffuse
	lightDir = normalize(-lightDir);
	float lightIntensity = saturate( dot(input.Normal, lightDir) );
	if(lightIntensity > 0.0f ) {
		color += diffuseColor * lightIntensity;
	}

    return saturate(color);
}