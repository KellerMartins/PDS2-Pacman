#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float width;
uniform float height;

out vec4 finalColor;

void main(void) {
    vec2 uv = fragTexCoord;
    vec4 color = texture(texture0, uv);

    float newPixel = 0.0;

    vec4 neighbor = texture2D(texture0, vec2(uv.x + 1/width,uv.y));
    vec3 maxn = max(color.rgb,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x - 1/width,uv.y));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x,uv.y + 1/height));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    neighbor = texture2D(texture0,vec2(uv.x,uv.y - 1/height));
    maxn = max(maxn,neighbor.rgb);
    newPixel += step(0.001,abs(neighbor.a - color.a));

    color.rgb = color.a < 1.0? maxn * newPixel/4.0 : color.rgb/4.0;
    
    finalColor = vec4(color.rgb,1.0);
}