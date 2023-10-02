#version 450
out vec4 FragColor;
uniform float iTime;
uniform vec2 iResolution;
uniform vec3 _daySkyColor;
uniform vec3 _dayGroundColor;
uniform vec3 _nightSkyColor;
uniform vec3 _nightGroundColor;
uniform vec3 _sunTopColor;
uniform vec3 _sunBottomColor;
uniform vec4 _skylineColor;
uniform float _sunSpeed;
uniform float _sunRadius;
uniform float _hillAmount;
uniform float _hillSteepness;
in vec2 UV;

void main()
{
    //Ratio and SDF
    vec2 fragCoord = vec2(gl_FragCoord.x, gl_FragCoord.y);
    float ratio = iResolution.x/iResolution.y;
    vec2 sdfUV = ((2.0*gl_FragCoord.xy)-iResolution.xy)/iResolution.y;  //This function is identical to the one below, but does not work
    sdfUV = (2.0*fragCoord-vec2(1080.0,720.0))/720.0;                   //Only this function works, for some reason

    //Skyline
    float sinWave1 = sin(UV.x*_hillAmount)*_hillSteepness + 0.25;
    float sinWave2 = sin(UV.x*(_hillAmount*0.5f))*(_hillSteepness*0.75f);
    float skyline = 1.0 - step(sinWave1+sinWave2, UV.y);

    // Sky Background
    vec3 dayGradient = (mix(_daySkyColor, _dayGroundColor, UV.y));
    vec3 nightGradient = (mix(_nightSkyColor, _nightGroundColor, UV.y));
    float mask = 0.5 + 0.5*sin(iTime*_sunSpeed);
    vec3 sky = mix(dayGradient, nightGradient, mask);

    //Sun
    vec3 sunColor = mix(_sunTopColor, _sunBottomColor, mask);
    vec2 center = vec2(0.0, ((-sin(iTime * _sunSpeed))*1.0)-0.5);
    float sunMask = length(sdfUV-center)-_sunRadius; //Add center and -radius back
    sunMask = 1.0 - smoothstep(-0.01,0.01,sunMask);

    // Output to screen
    vec3 col = mix(sky, sunColor, sunMask);
    col = mix(col, _skylineColor.rgb, skyline*_skylineColor.a);
	FragColor = vec4(col,1.0);
}