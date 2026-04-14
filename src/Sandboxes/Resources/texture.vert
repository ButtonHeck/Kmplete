#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec2 outUV;
layout (location = 1) out float outLodBias;
layout (location = 2) out int outTiling;

layout (set = 0, binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 modelMatrix;
    float lodBias;
    int tiling;
} uboMatrices;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	outUV = inUV;
    outLodBias = uboMatrices.lodBias;
    outTiling = uboMatrices.tiling;
	gl_Position = uboMatrices.projectionMatrix * uboMatrices.viewMatrix * uboMatrices.modelMatrix * vec4(inPos.xyz, 1.0);
}
