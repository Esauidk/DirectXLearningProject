cbuffer LightCBuf {
	float3 lightPos;
}

// Colors
static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float3 ambientColor = { 0.05f, 0.05f, 0.05f };

static const float diffuseIntensity = 1.0f;
static const float constAttenuation = 1.0f;
static const float linearAuttenuation = 0.045f;
static const float quadraticAuttenuation = 0.0075f;



float4 main(float3 wordPos : Position, float3 normal : Normal) : SV_TARGET
{
	// Calculating unit vector from this pixel and the light source
    const float3 lightDirectionVector = lightPos - wordPos;
    const float lightDistance = length(lightDirectionVector);
    const float3 normLightDirectionVector = lightDirectionVector / lightDistance;
	
	// Calculating Attenuation (Loss of Intensity over distance)
    const float attenuation = 1.0f / (constAttenuation + linearAuttenuation * lightDistance + quadraticAuttenuation * (lightDistance * lightDistance));

	// Calculating the diffuse lighting (How light spreads on the surface)
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(normal, normLightDirectionVector));
	

	return float4(saturate(diffuse + ambientColor), 1.0f);
}