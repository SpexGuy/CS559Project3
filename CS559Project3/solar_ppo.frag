#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

void main() {
	vec4 color = texture(textureIndex, vec2(gl_FragCoord)/size);
	FragColor = (0.2126*color.r + 0.7152*color.g + 0.0722*color.b) > 0.5 ? vec4(vec3(1.0) - vec3(color), color.w) : color;
}