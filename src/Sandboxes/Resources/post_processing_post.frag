#version 450

layout (set = 0, binding = 0) uniform texture2D frameTexture;
layout (set = 0, binding = 1) uniform sampler textureSampler;
layout (set = 0, binding = 2) uniform UBO
{
    float width;
    float height;
} ubo;

layout (location = 0) out vec4 outFragColor;

void make_kernel(inout vec4 n[9], texture2D tex, sampler smp, vec2 coord)
{
	float w = 1.0 / ubo.width;
	float h = 1.0 / ubo.height;

	n[0] = texture(sampler2D(tex, smp), coord + vec2( -w, -h));
	n[1] = texture(sampler2D(tex, smp), coord + vec2(0.0, -h));
	n[2] = texture(sampler2D(tex, smp), coord + vec2(  w, -h));
	n[3] = texture(sampler2D(tex, smp), coord + vec2( -w, 0.0));
	n[4] = texture(sampler2D(tex, smp), coord);
	n[5] = texture(sampler2D(tex, smp), coord + vec2(  w, 0.0));
	n[6] = texture(sampler2D(tex, smp), coord + vec2( -w, h));
	n[7] = texture(sampler2D(tex, smp), coord + vec2(0.0, h));
	n[8] = texture(sampler2D(tex, smp), coord + vec2(  w, h));
}

void main(void) 
{
	vec4 n[9];
	make_kernel(n, frameTexture, textureSampler, gl_TexCoord[0].st);

	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	outFragColor = vec4( 1.0 - sobel.rgb, 1.0 );
}