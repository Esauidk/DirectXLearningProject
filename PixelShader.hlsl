cbuffer CBuf {
	float4 face_colors[6];
};

float4 main(float3 color : COLOR) : SV_TARGET //uint tid : SV_PrimitiveID) : SV_TARGET
{
	return float4(color, 1.0f); //face_colors[tid/2];
}
