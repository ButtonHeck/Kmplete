#version 450

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inPosInstanced;
layout (location = 2) in vec4 inColorInstanced;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = inColorInstanced;
    gl_Position = vec4(inPos.xy + inPosInstanced.xy, 0.0, 1.0);
}