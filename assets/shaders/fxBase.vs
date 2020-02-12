#version 100
precision mediump float;

attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

uniform mat4 mvp;

varying  vec2 fragTexCoord;
varying  vec4 fragColor;


void main()
{
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}