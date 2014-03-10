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
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Position : POSITION, float4 Color : COLOR, float4 Normal : NORMAL )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	// super dumb lighting calc
	output.Color = Color * Normal + ( 0.1f, 0.1f, 0.1f, 0.2f );
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}