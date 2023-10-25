#include "Object3d.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	if (gMaterial.enableLighting == 1) {
		float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else if (gMaterial.enableLighting == 2) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
		output.color.a = gMaterial.color.a * textureColor.a;
	}

	if (textureColor.a <= 0.5 || output.color.a == 0) {
		discard;
	}
	
	
	return output;
}