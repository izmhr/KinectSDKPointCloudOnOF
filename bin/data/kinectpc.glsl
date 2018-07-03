#version 430

layout(std430, binding=0) buffer colorSpacePoints {
	vec2 p[];
};

layout(std430, binding=1) buffer colors {
	uint c[];
};

layout(std430, binding=2) buffer colorsResult {
	vec4 cr[];
};

uniform uint CWIDTH;
uniform uint CHEIGHT;
uniform uint DWIDTH;
uniform uint DHEIGHT;

layout(local_size_x = 8, local_size_y = 1, local_size_z = 1) in;
void main() {
	//uint index = gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_GlobalInvocationID.y + gl_GlobalInvocationID.x;
	uint index = gl_GlobalInvocationID.x;
	//uint index = 0;
	vec2 colorpos = p[index];
	//vec3 colorpos;
	//colorpos.x = 50; colorpos.y = 80;
	//uint colorIdx = 540 * 1920 - 1280;
	uint colorIdx = uint(colorpos.y) * 1920 + uint(colorpos.x);
	uint intcolor = c[colorIdx];
	uint a = intcolor / (256 * 256 * 256);
	////uint r = 255;
	//uint g = 0;
	//uint b = 0;
	//uint a = 255;
	intcolor = intcolor - a * 256 * 256 * 256;
	uint b = intcolor / (256 * 256);
	intcolor = intcolor - b * 256 * 256;
	uint g = intcolor / 256;
	uint r = intcolor - g * 256;
	//uint r = 0;
	//uint g = aa;
	//uint b = 0;
	//uint a = 255;
	//cr[index].r = float(r) / 255.0 / 255.0 / 255.0 / 255.0;
	//cr[index].g = float(g) / 255.0 / 255.0 / 255.0 / 255.0;
	//cr[index].b = float(b) / 255.0 / 255.0 / 255.0 / 255.0;
	//cr[index].a = 1.0;
	cr[index].r = float(r) / 256.0;
	cr[index].g = float(g) / 256.0;
	cr[index].b = float(b) / 256.0;
	cr[index].a = 1.0;
	//cr[index].r = 0;
	//cr[index].g = 0;
	//cr[index].b = 1.0;
	//cr[index].a = 1.0;
}