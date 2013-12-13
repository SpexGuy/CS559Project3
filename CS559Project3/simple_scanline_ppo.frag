#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

void main(){
	vec4 color = texture(textureIndex, vec2(gl_FragCoord)/size);
	FragColor = ((int(gl_FragCoord.y/2) % 2 == 0) ? color : color/2.0);
}