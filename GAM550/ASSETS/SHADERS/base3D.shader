cbuffer ConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	float4 CameraPosition;
	float4 LightPosition;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float3x3 tbn : TBN;
	float4 view : VIEW;
	float4 light : LIGHT;
	float4 color : COLOR;
};


VOut VShader(
	float4 position : POSITION,
	float4 normal : NORMAL,
	float4 tangent : TANGENT,
	float4 bitangent : BITANGENT,
	float2 texCoords : TEXCOORDS,
	float4 color : COLOR)
{
	VOut output;
	float4 P = mul(ModelMatrix, position);
	float4 n = mul(NormalMatrix, normal);
	float4 t = mul(NormalMatrix, tangent);
	float4 b = mul(NormalMatrix, bitangent);

	float3 T = normalize(mul(ModelMatrix, tangent)).xyz;
	float3 B = normalize(mul(ModelMatrix, bitangent)).xyz;
	float3 N = normalize(mul(ModelMatrix, normal)).xyz;


	output.position = mul(MatFinal, position);
	output.normal = normalize(normal);
	output.tbn = float3x3(T, B, N);
	output.view = CameraPosition - P;
	output.light = LightPosition - P;
	output.color = color;

	return output;
}


float4 PShader(
	float4 position : SV_POSITION, 
	float4 normal : NORMAL, 
	float3x3 tbn : TBN,
	float4 view : VIEW,
	float4 light : LIGHT,
	float4 color : COLOR
) : SV_TARGET
{
	float4 ambient = float4(0.2, 0.2, 0.2, 1);
	float4 diffuse = max(dot(normal, light), 0) * color;
	return diffuse + ambient;
}