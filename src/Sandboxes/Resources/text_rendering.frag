#version 450

layout (set = 0, binding = 0) uniform sampler _sampler;
layout (set = 0, binding = 1) uniform texture2D _texture;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

const vec3 textColor = vec3(1.0, 1.0, 1.0);

void main()
{
    float distance = texture(sampler2D(_texture, _sampler), inUV).r;
    float edgeWidth = fwidth(distance);
    float alpha = smoothstep(0.5 - edgeWidth, 0.5 + edgeWidth, distance);
    if (alpha < 0.01)
    {
        discard;
    }

    outFragColor = vec4(textColor, alpha);
}