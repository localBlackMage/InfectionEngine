cbuffer ConstantBuffer
{
	float4x4 ModelMatrix;
	float4 Ambient;
};

struct VertexInput {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 bitangent : BITANGENT;
	float2 texCoords : TEXCOORDS;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

Texture2D Texture;
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;

	output.position = mul(ModelMatrix, input.position);
	output.texCoords.x = output.position.x * 0.5 + 0.5;
	output.texCoords.y = output.position.y * -0.5 + 0.5;

	return output;
}

float4 PShader(PixelInput input) : SV_TARGET
{
	float4 color = Texture.Sample(ss, input.texCoords);
	//color.a = 1;
	//color.g = color.b = 0;
	return color * Ambient;
	
}