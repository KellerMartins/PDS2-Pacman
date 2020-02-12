#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 colDiffuse;
uniform float width;
uniform float height;

uniform float vignettePower;

varying vec4 finalColor;

float when_gt(float x, float y) {
   return max(sign(x - y), 0.0);
}


//From https://stackoverflow.com/questions/12105330/how-does-this-simple-fxaa-work
vec3 FXAA(sampler2D tex, vec2 texCoords, vec2 pixelSize){
	float FXAA_SPAN_MAX = 8.0;
    float FXAA_REDUCE_MUL = 1.0/8.0;
    float FXAA_REDUCE_MIN = 1.0/128.0;

    vec3 rgbNW=texture2D(tex,texCoords+(vec2(-1.0,-1.0)*pixelSize)).xyz;
    vec3 rgbNE=texture2D(tex,texCoords+(vec2(1.0,-1.0)*pixelSize)).xyz;
    vec3 rgbSW=texture2D(tex,texCoords+(vec2(-1.0,1.0)*pixelSize)).xyz;
    vec3 rgbSE=texture2D(tex,texCoords+(vec2(1.0,1.0)*pixelSize)).xyz;
    vec3 rgbM=texture2D(tex,texCoords).xyz;

    vec3 luma=vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) * pixelSize;

    vec3 rgbA = (1.0/2.0) * (
        texture2D(tex, texCoords.xy + dir * (1.0/3.0 - 0.5)).xyz +
        texture2D(tex, texCoords.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture2D(tex, texCoords.xy + dir * (0.0/3.0 - 0.5)).xyz +
        texture2D(tex, texCoords.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax)){
        return rgbA;
    }else{
        return rgbB;
    }
}

vec3 tonemap(vec3 x)
{
    //Reinhard
    //return x / (x + 1.0);

    //ACESFilm
    return clamp((x*(2.51*x+0.03))/(x*(2.43*x+0.59)+0.14), 0.0, 1.0);

    //Filmic
    //x = max(vec3(0.0), x - 0.004);
	//return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);

    //Uncharted 2
    //return ((x*(0.15*x+0.10*0.50)+0.20*0.02)/(x*(0.15*x+0.50)+0.20*0.30))-0.02/0.30;
}

void main(void) {

    vec2 uv = fragTexCoord;

    //Vignette
    vec3 dist = vec3((uv.x - 0.5) * 1.25,(uv.y - 0.5) * 1.25,0.0);
    float vignette = clamp(1.0 - dot(dist, dist)*vignettePower,0.0,1.0);

    vec3 sharp = FXAA(texture0, fragTexCoord, vec2(1.0/width, 1.0/height)).rgb;
    vec3 blur = texture2D(texture1, fragTexCoord).rgb;

    const float expos = 0.75;
    vec3 pixel = (sharp*3.0 + blur*5.0);

    //Pseudo HDR
    float maxVal = max(pixel.r, max(pixel.g, pixel.b));
    pixel.rgb += when_gt(maxVal,1.0)*vec3(maxVal - 1.0);

    gl_FragColor = vec4(vignette*pixel,1.0);
}