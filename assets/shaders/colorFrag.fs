#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 mask;
uniform vec3 color;

void main()
{     
    finalColor = vec4(colDiffuse.rgb,1.0);
}