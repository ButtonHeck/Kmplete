#version 450

layout (location = 0) in vec3 inColor;

layout (binding = 0) uniform UBO 
{
    float colorMultiplier;
} ubo;

layout (location = 0) out vec4 outFragColor;

void main() 
{
    outFragColor = vec4(inColor * ubo.colorMultiplier, 1.0);
}