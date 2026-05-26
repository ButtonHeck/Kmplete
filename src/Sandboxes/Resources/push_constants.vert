#version 450

layout (location = 0) in vec2 inPos;

layout (push_constant) uniform PushConstants{
    vec4 position;
    vec4 color;
} pushConstants;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = pushConstants.color;
    vec2 pos = inPos + pushConstants.position.xy;
    gl_Position = vec4(pos, 0.0, 1.0);
}