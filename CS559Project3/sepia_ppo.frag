#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

void main() {
	vec4 color = texture(textureIndex, vec2(gl_FragCoord)/size);
	FragColor = 
		vec4((color.r * 0.393) + (color.g * 0.769) + (color.b * 0.189),
		(color.r * 0.349) + (color.g * 0.686) + (color.b * 0.168),
		(color.r * 0.272) + (color.g * 0.534) + (color.b * 0.131),
		color.a);
}