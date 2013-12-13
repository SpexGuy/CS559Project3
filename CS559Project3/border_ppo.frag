#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;

void main(){
	vec2 uv = vec2(gl_FragCoord)/size;
	if (gl_FragCoord.x < 10 || gl_FragCoord.y < 10 || size.x - gl_FragCoord.x < 10 || size.y - gl_FragCoord.y < 10)
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else
		FragColor = texture(textureIndex, (gl_FragCoord.xy - vec2(10))/(size - vec2(20)));
}