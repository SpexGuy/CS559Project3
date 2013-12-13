#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

void main(){
	FragColor = texture(textureIndex, vec2(gl_FragCoord)/size);
}