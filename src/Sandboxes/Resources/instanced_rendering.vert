#version 450

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inPosInstanced;

void main()
{
    gl_Position = vec4(inPos.xy + inPosInstanced.xy, 0.0, 1.0);
}