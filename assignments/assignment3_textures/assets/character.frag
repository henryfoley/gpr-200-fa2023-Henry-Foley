#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CharacterTexture;

void main(){
	FragColor = texture(_CharacterTexture,UV);
	//FragColor = vec4(UV,0.0,1.0);
}