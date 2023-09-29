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
in vec2 UV;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 fragCoord = vec2(gl_FragCoord.x, gl_FragCoord.y);
    float ratio = iResolution.x/iResolution.y;
    vec2 sdfUV = (2.0*fragCoord-iResolution.xy)/iResolution.y;

    //Skyline
    float skyline = 1.0 - step(sin(UV.x*15.0)*0.1 + 0.25, UV.y);
    //skyline = mod(floor(uv.x*12.0),2.0);
    //float stripes = sin(floor(12.0 * uv.x) - sin(floor(15.0* uv.x)) - sin(floor(5.0 * uv.x * uv.x)));
    //skyline = 0.7 * skyline + 0.3 * stripes;
    //skyline = floor(skyline);


    // Sky Background
    vec3 dayGradient = (mix(_daySkyColor, _dayGroundColor, UV.y));
    vec3 nightGradient = (mix(_nightSkyColor, _nightGroundColor, UV.y));
    float mask = 0.5 + 0.5*sin(iTime*_sunSpeed);
    vec3 sky = mix(dayGradient, nightGradient, mask);


    //Sun
    vec3 sunColor = mix(_sunTopColor, _sunBottomColor, mask);
    vec2 center = vec2(0.5, (-sin(iTime * _sunSpeed))*0.5);
    float sunMask = length(center-UV)-_sunRadius;
    sunMask = 1.0 - smoothstep(-0.01,0.01,sunMask);

    // Output to screen
    vec3 col = mix(sky, sunColor, sunMask);
    //col = sky;
    col = mix(col, _skylineColor.rgb, skyline*_skylineColor.a);
	FragColor = vec4(col,1.0);
    //FragColor = vec4(UV,0.0,1.0);
}