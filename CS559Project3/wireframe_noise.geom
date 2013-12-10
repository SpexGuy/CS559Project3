//Blatantly stolen from GLSL cookbook
#version 400
layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;
out vec3 GPPosition;
out vec3 GVPosition;
out vec3 GWPosition;
out vec3 GNormal;
out vec2 GUV;
noperspective out vec3 GEdgeDistance;
in vec2 VUV[];
in vec3 VPPosition[];
in vec3 VWPosition[];
in vec3 VVPosition[];
in vec3 VNormal[];
uniform mat4 ViewportMatrix; // Viewport matrix
void main()
{
	// Transform each vertex into viewport space
	vec3 p0 = vec3(ViewportMatrix * (gl_in[0].gl_Position /
	gl_in[0].gl_Position.w));
	vec3 p1 = vec3(ViewportMatrix * (gl_in[1].gl_Position /
	gl_in[1].gl_Position.w));
	vec3 p2 = vec3(ViewportMatrix * (gl_in[2].gl_Position /
	gl_in[2].gl_Position.w));

	// Find the altitudes (ha, hb and hc)
	float a = length(p1 - p2);
	float b = length(p2 - p0);
	float c = length(p1 - p0);
	float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
	float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
	float ha = abs( c * sin( beta ) );
	float hb = abs( c * sin( alpha ) );
	float hc = abs( b * sin( alpha ) );

	// Send the triangle along with the edge distances
	GEdgeDistance = vec3( ha, 0, 0 );
	GWPosition = VWPosition[0];
	GVPosition = VVPosition[0];
	GPPosition = VPPosition[0];
	GUV = VUV[0];
	GNormal = VNormal[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	GEdgeDistance = vec3( 0, hb, 0 );
	GWPosition = VWPosition[1];
	GVPosition = VVPosition[1];
	GPPosition = VPPosition[1];
	GUV = VUV[1];
	GNormal = VNormal[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	GEdgeDistance = vec3( 0, 0, hc );
	GWPosition = VWPosition[2];
	GVPosition = VVPosition[2];
	GPPosition = VPPosition[2];
	GUV = VUV[2];
	GNormal = VNormal[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}