#version 400

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

 
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex;
 
// Output data ; will be interpolated for each fragment.
out vec2 VUV;
out vec3 VNormal;
out vec3 VPosition;

 
void main(){
 	VNormal = normalize(normal_matrix * vertex_normal);

	//Position is the vertex position in eye space.
	VPosition = vec3(modelview_matrix * vec4(vertex_position,1.0));
	//position in space
	gl_Position = mvp * vec4(vertex_position, 1.0);

	VUV = vertex_tex;
}