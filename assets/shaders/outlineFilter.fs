#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float width;
uniform float height;

varying vec4 finalColor;

void main(void) {
    vec2 uv = fragTexCoord;
    vec4 color = texture2D(texture0, uv);

    float newPixel = 0.0;

    vec4 neighbor = texture2D(texture0, vec2(uv.x + 1.0/width,uv.y));
    vec3 maxn = max(color.rgb,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x - 1.0/width,uv.y));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x,uv.y + 1.0/height));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x,uv.y - 1.0/height));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    color.rgb = color.a < 1.0? maxn * newPixel/4.0 : color.rgb/4.0;
    
    gl_FragColor = vec4(color.rgb,1.0);
}