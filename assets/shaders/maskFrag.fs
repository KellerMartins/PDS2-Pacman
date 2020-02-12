#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;
varying vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 mask;
uniform vec3 color;

void main()
{     
    vec4 tex = texture2D(texture0, fragTexCoord); 
    gl_FragColor = vec4(colDiffuse.rgb, (tex.r + tex.g + tex.b)/3.0);
}