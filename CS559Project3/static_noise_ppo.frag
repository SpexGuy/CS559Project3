#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;
uniform vec2 size;
uniform float time;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	vec4 color = texture(textureIndex, vec2(gl_FragCoord)/size);
	FragColor = vec4(vec3((0.1 + 0.1 * (rand(vec2(rand(gl_FragCoord.xy),time)) > 0.1 ? 0.0 : -1.0)) + color.xyz), 1.0);
}