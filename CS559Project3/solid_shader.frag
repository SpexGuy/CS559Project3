//Blatantly copied from GLSL Cookbook (the wireframe part, at least.)
//Also temporarily green for now. so yeah.
#version 400

layout (location = 0) out vec4 FragColor;
noperspective in vec3 GEdgeDistance;
flat in vec4 GColor;
uniform bool wireframe = true;

void main() {
	float d = min(GEdgeDistance.x, GEdgeDistance.y);
	d = min(d, GEdgeDistance.z);
	float mixval = smoothstep(0,2,d);
	if(wireframe)
	{
	FragColor = mix(vec4(0.0f,1.0f,0.0f,1.0f),GColor, mixval);
	}
	else
	{
	FragColor = GColor;
	}
}
