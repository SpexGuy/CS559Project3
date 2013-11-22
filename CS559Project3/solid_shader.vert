#version 400

layout (location = 0) in vec3 vertex_position;


uniform mat4 mvp;
uniform vec4 color;

flat out vec4 VColor;
out vec3 VPosition;


void main() {
	gl_Position = mvp * vec4(vertex_position, 1.0);
	VPosition = vec3(gl_Position);
	VColor = color;
}
