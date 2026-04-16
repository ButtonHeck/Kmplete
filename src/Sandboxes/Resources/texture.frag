#version 450

layout (set = 0, binding = 1) uniform texture2D _texture;
layout (set = 1, binding = 0) uniform sampler _sampler;

layout (location = 0) in vec2 inUV;
layout (location = 1) in float inLodBias;
layout (location = 2) flat in int inTiling;

layout (location = 0) out vec4 outFragColor;

void main() 
{
    vec4 color = texture(sampler2D(_texture, _sampler), inUV * inTiling, inLodBias);
    outFragColor = color;
}