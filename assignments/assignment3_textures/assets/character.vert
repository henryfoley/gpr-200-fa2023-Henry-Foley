#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _Scale;
uniform float _Time;
uniform float _XOffset;
uniform float _YOffset;
uniform float _JumpSpeed;
out vec2 UV;
void main(){

	UV = vUV/_Scale;
	//UV.x += _XOffset;
	//UV.y += _YOffset;
	UV = UV * 2 - 0.5;

	vec3 offset = vec3(_XOffset,sin(_Time*_JumpSpeed)*0.5+_YOffset,0);
	gl_Position = vec4(vPos+offset,1.0);
}