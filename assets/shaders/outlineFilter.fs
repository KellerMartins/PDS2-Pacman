#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;
uniform float width;
uniform float height;

out vec4 finalColor;

void main(void) {

    vec4 allOnes = vec4(1);

    vec2 uv = fragTexCoord;
    float mask = dot(texture(texture0, uv), allOnes)/3.0;
    vec3 color = texture(texture1, uv).rgb;

    float newPixel = 0.0;

    float neighbor = dot(texture2D(texture0, vec2(uv.x + 1/width,uv.y)), allOnes)/3.0;
    vec3 neighborColor = texture2D(texture1, vec2(uv.x + 1/width,uv.y)).rgb;
    vec3 maxn = max(color,neighborColor);
    newPixel += step(0.001,abs(neighbor - mask));

    neighbor = dot(texture2D(texture0, vec2(uv.x - 1/width,uv.y)), allOnes)/3.0;
    neighborColor = texture2D(texture1,vec2(uv.x - 1/width,uv.y)).rgb;
    maxn = max(maxn,neighborColor);
    newPixel += step(0.001,abs(neighbor - mask));

    neighbor = dot(texture2D(texture0, vec2(uv.x,uv.y + 1/height)), allOnes)/3.0;
    neighborColor = texture2D(texture1,vec2(uv.x,uv.y + 1/height)).rgb;
    maxn = max(maxn,neighborColor);
    newPixel += step(0.001,abs(neighbor - mask));

    neighbor = dot(texture2D(texture0, vec2(uv.x,uv.y - 1/height)), allOnes)/3.0;
    neighborColor = texture2D(texture1,vec2(uv.x,uv.y - 1/height)).rgb;
    maxn = max(maxn,neighborColor);
    newPixel += step(0.001,abs(neighbor - mask));

    color = mask < 1.0? maxn * newPixel/4.0 : color/4.0;

    finalColor = vec4(color,1.0);
}