cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 cameraPos : Position;
    float3 normal : Normal;
    float2 texCord : TexCoord;
    float4 pos : SV_Position;
};
VSOut main(float3 pos : Position, float3 normal : Normal, float2 texCord : TexCoord)
{
    VSOut vso;
    vso.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(normal, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.texCord = texCord;
	return vso;
}