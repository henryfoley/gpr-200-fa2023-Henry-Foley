#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; //Per-Fragment interpolated UV
	vec3 WorldPosition; //Per-Fragment interpolated World Position
	vec3 WorldNormal; //Per-Fragment interpolated World Normal
}fs_in;

struct Light{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4

uniform Light _Lights[MAX_LIGHTS];
uniform sampler2D _Texture;



void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	FragColor = texture(_Texture,fs_in.UV);
}