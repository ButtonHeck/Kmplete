#version 450

layout (location = 0) in vec3 inPos;

layout (location = 0) out vec4 outColor;

layout (std430, binding = 0) readonly buffer Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model[];
} matrices;

layout (std430, binding = 1) readonly buffer Color
{
    vec4 color;
} color;

void main()
{
    outColor = color.color;
    gl_Position = matrices.projection * matrices.view * matrices.model[gl_InstanceIndex] * vec4(inPos, 1.0);
}