#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

#define KERNEL_SIZE 7
#define KERNEL_MID 3
const float kernel[KERNEL_SIZE] = float[KERNEL_SIZE](0.006,0.061,0.242,0.383,0.242,0.061,0.006);

void main(){
	vec4 sum = vec4(0);
	for (int c = 0; c < KERNEL_SIZE; c++) {
		sum += kernel[c]*texture(textureIndex, (gl_FragCoord.xy + vec2(0,c - KERNEL_MID))/size);
	}
	FragColor = sum;
}