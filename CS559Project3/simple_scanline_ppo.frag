#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;
uniform float time;

void main(){
	vec4 color = texture(textureIndex, vec2(gl_FragCoord)/size);
	FragColor = 0.8*color+0.2*color*sin(gl_FragCoord.y+5*time);
}