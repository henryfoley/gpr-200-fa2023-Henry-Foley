#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _BrickTexture;
uniform sampler2D _NoiseTexture;
uniform float _Time;
uniform float _Tiling;
uniform float _Distortion;
uniform float _DistortionSpeed;

void main(){
	float noise = texture(_NoiseTexture,UV/_Tiling).r;
	vec2 uv = UV + noise * _Distortion;
	uv += _Time*_DistortionSpeed;

	FragColor = texture(_BrickTexture,uv);
	//FragColor = vec4(UV,0.0,1.0);
}