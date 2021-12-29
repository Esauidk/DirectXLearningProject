struct VSOut {
	float3 color : Color;
	float4 pos : SV_Position;
	
};

cbuffer CBuf {
	matrix transform;
};

VSOut main( float3 pos : POSITION, float3 color: COLOR) //: SV_Position
{	
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), transform);
	vso.color = color;
	return vso;
}
