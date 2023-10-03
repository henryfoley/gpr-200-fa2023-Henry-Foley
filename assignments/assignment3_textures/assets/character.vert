#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _Scale;
uniform float _Time;
out vec2 UV;
void main(){

	UV = vUV/_Scale;
	UV = UV * 2 - 0.5;
	vec3 offset = vec3(0,sin(vPos.y + _Time),0)*0.5;
	gl_Position = vec4(vPos+offset,1.0);
}