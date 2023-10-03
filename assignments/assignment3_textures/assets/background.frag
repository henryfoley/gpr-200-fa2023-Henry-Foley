#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CrowdTexture;
uniform sampler2D _NoiseTexture;
uniform sampler2D _FloorTexture;
uniform float _Time;
uniform float _Tiling;
uniform float _Distortion;
uniform float _DistortionSpeed;

void main(){
	float noise = texture(_NoiseTexture,UV/_Tiling).r;
	vec2 originalUV = UV/_Tiling;
	vec2 uv = UV + noise * _Distortion;
	uv += _Time*_DistortionSpeed;
	float mask = step(UV.y/_Tiling,0.25);
	FragColor = mix(texture(_CrowdTexture,uv),texture(_FloorTexture,originalUV),mask);
	//FragColor = vec4(UV,0.0,1.0);
}