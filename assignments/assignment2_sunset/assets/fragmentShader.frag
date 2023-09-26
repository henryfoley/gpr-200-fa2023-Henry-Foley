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
    vec2 uv = fragCoord/iResolution.xy;
    float ratio = iResolution.x/iResolution.y;

    /*//Skyline
    float skyline = 1.0 - step(sin(uv.x*15.0)*0.1 + 0.25, uv.y);
    //skyline = mod(floor(uv.x*12.0),2.0);
    float stripes = sin(floor(12.0 * uv.x) - sin(floor(15.0* uv.x)) - sin(floor(5.0 * uv.x * uv.x)));
    skyline = 0.7 * skyline + 0.3 * stripes;
    //skyline = floor(skyline);*/


    // Sky Background
    vec3 dayGradient = (mix(_daySkyColor, _dayGroundColor, uv.y));
    vec3 nightGradient = (mix(_nightSkyColor, _nightGroundColor, uv.y));
    float mask = 0.5 + 0.5*sin(iTime*_sunSpeed);
    vec3 sky = mix(dayGradient, nightGradient, mask);
    vec3 sunColor = mix(_sunTopColor, _sunBottomColor, mask);

    //Sky and Sun
    vec2 center = vec2(iResolution.x * 0.5, (iResolution.y*1.25 * -sin(iTime * _sunSpeed))*0.5);
    uv = fragCoord.xy;
    float radius = 0.25 * iResolution.y;
    center = uv - center;
    float sunMask = length(center)-radius;
    sunMask = 1.0 - smoothstep(-10.0,10.0,sunMask);

    // Output to screen
    vec3 col = mix(sky, sunColor, sunMask);
    //col = mix(col, skylineColor.rgb, skyline*skylineColor.a);
	FragColor = vec4(col,1.0);
}