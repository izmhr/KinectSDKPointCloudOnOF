#version 430

layout(std430, binding=0) buffer colorSpacePoints {
	vec2 colorSpace[];
};

layout(std430, binding=1) buffer colors {
	uint color[];
};

layout(std430, binding=2) buffer colorsResult {
	vec4 colorResult[];
};

uniform int CWIDTH;
//uniform int CHEIGHT;
//uniform int DWIDTH;
//uniform int DHEIGHT;

layout(local_size_x = 8, local_size_y = 1, local_size_z = 1) in;
void main() {
	// index はいま全ループ(512x424…デプス画像のピクセル数)の内
	// 何番目をやっているかを表す
	uint index = gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_GlobalInvocationID.y + gl_GlobalInvocationID.x;
	//uint index = gl_GlobalInvocationID.x;

	// colorSpace は、index番目のデプス画像が color 画像のどの位置を指すか
	// の対応付けをしてくれる
	vec2 colorPos = colorSpace[index];

	// 更に、全画像(1920x1080)中の何番目のピクセルか、に変換する
	uint colorIdx = uint(colorPos.y) * CWIDTH + uint(colorPos.x);
	
	// rgba の順で byte 列に渡されてきた色情報を
	// 4byte ずつまとめて uint として受け取っているので
	// それを分割する
	uint intcolor = color[colorIdx];
	uint a = intcolor / (256 * 256 * 256);
	intcolor = intcolor - a * 256 * 256 * 256;
	uint b = intcolor / (256 * 256);
	intcolor = intcolor - b * 256 * 256;
	uint g = intcolor / 256;
	uint r = intcolor - g * 256;

	// 0~255 で色が決まるので 255 で割る
	colorResult[index].r = float(r) / 255.0;
	colorResult[index].g = float(g) / 255.0;
	colorResult[index].b = float(b) / 255.0;
	colorResult[index].a = 1.0;

	// coloResult は VBO でも共有して利用される
}