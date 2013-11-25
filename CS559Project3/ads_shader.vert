#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 color;

flat out vec4 VColor;
out vec3 VNormal;
out vec3 VPosition;
 

void main()
{
	//get the vertex color
	VColor = color;
	VNormal = normalize(normal_matrix * vertex_normal);
	//color = normal is a cool shader.
	//color = normal;


	//Position is the vertex position in eye space.
	VPosition = vec3(modelview_matrix * vec4(vertex_position,1.0));
	//position in space
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
