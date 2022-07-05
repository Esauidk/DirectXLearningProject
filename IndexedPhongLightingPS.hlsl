cbuffer LightCBuf
{
    float3 lightPos;
    float3 diffuseColor;
    float3 ambientColor;
    float diffuseIntensity;
    float constAttenuation;
    float linearAuttenuation;
    float quadraticAuttenuation;
};

cbuffer ObjectCBuf
{
    float3 materialColor[6];
    float padding;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 cameraPos : Position, float3 normal : Normal, uint tid : SV_PrimitiveID) : SV_TARGET
{
    // Calculating unit vector from this pixel relative to camera and the light source
    const float3 lightDirectionVector = lightPos - cameraPos;
    const float lightDistance = length(lightDirectionVector);
    const float3 normLightDirectionVector = lightDirectionVector / lightDistance;
	
	// Calculating Attenuation (Loss of Intensity over distance)
    const float attenuation = 1.0f / (constAttenuation + linearAuttenuation * lightDistance + quadraticAuttenuation * (lightDistance * lightDistance));

	// Calculating the diffuse lighting (How light spreads on the surface)
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(normal, normLightDirectionVector));
    
    // Reflecting light vector;
    const float3 reflectVector = reflect(-lightDirectionVector, normal);
    
    // Calculate the angle from the reflection vector and camera position
    const float specularAngle = max(0.0f, dot(normalize(reflectVector), normalize(cameraPos)));
    const float specularCoefficient = pow(specularAngle, specularPower);
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity * specularCoefficient;
    
	return float4(saturate((diffuse + ambientColor + specular) * materialColor[tid/2 % 6]), 1.0f);
}