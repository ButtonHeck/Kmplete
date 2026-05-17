#version 450

layout (location = 0) in vec3 inPos;

layout (binding = 0) uniform UboView 
{
	mat4 projection;
	mat4 view;
} uboView;

layout (binding = 1) uniform UboInstance 
{
	mat4 model; 
} uboInstance;

void main()
{
    gl_Position = uboView.projection * uboView.view * uboInstance.model * vec4(inPos.xyz, 1.0);
}