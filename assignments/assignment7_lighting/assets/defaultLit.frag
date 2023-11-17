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
	float intensity;
};
#define MAX_LIGHTS 4

uniform Light _Lights[MAX_LIGHTS];
uniform sampler2D _Texture;
uniform vec3 _CameraPos;

//Material Properties
uniform float _AmbientK; 
uniform float _DiffuseK;
uniform float _Specular;
uniform float _Shininess;


void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec4 result = vec4(0,0,0,0.0);
	float intensity = 0.5f;
	for(int i = 0; i < MAX_LIGHTS; i++){

		//Position Calculations
		vec3 lightDir = normalize(_Lights[i].position - fs_in.WorldPosition);
		vec3 viewDir = normalize(_CameraPos - fs_in.WorldPosition);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		//Ambient
		vec3 ambient = _Lights[i].color * _AmbientK;

		//Diffuse
		float diff = max(dot(normal, lightDir), 0.0) * _DiffuseK;
		vec3 diffuse = _Lights[i].color * diff;
		
		//Specular
		float spec = pow(max(dot(normal, halfwayDir), 0.0), _Shininess) * _Specular;
		vec3 specular = _Lights[i].color * spec;

		//Combine to Blinn Phong
		result += vec4((diffuse + specular + ambient), 1.0);
		result *= _Lights[i].intensity;
	}

	FragColor =  result * texture(_Texture,fs_in.UV);
}