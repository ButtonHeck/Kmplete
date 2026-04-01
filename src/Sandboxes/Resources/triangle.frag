#version 450

layout (location = 0) in vec4 inColor;

layout (set = 1, binding = 3) uniform UBO 
{
    float colorMultiplier;
} ubo;

layout (location = 0) out vec4 outFragColor;

void main() 
{
    outFragColor = vec4(inColor.rgb * ubo.colorMultiplier, inColor.a);
}