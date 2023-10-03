#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CharacterTexture;
uniform float _Time;

void main(){
    vec2 sdfUV = (2.0*gl_FragCoord.xy-vec2(1080.0,720.0))/720.0; 
	FragColor = texture(_CharacterTexture,UV);
	//FragColor = vec4(UV,0.0,1.0);
}