#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 modelMatrix;
} uboMatrices;

void main() 
{
	outColor = inColor;
	gl_Position = uboMatrices.projectionMatrix * uboMatrices.viewMatrix * uboMatrices.modelMatrix * vec4(inPos.xyz, 1.0);
}
