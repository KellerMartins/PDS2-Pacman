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
    gl_FragColor = vec4(colDiffuse.rgb,1.0);
}