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
    vec4 tex = texture(texture0, fragTexCoord); 
    finalColor = vec4(colDiffuse.rgb, (tex.r + tex.g + tex.b)/3.0);
}