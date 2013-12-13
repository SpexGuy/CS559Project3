#version 400
// This shader comes from https://www.shadertoy.com/view/MdXGDH

const float PI = 3.14159265;

uniform float time;

void main(void ) {
	float ttime = time * 0.2;
	
	
	float color1, color2, color;
	
	color1 = (sin(dot(gl_FragCoord.xy,vec2(sin(ttime*3.0),cos(ttime*3.0)))*0.02+ttime*3.0)+1.0)/2.0;
	
	vec2 center = vec2(640.0/2.0, 360.0/2.0) + vec2(640.0/2.0*sin(-ttime*3.0),360.0/2.0*cos(-ttime*3.0));
	
	color2 = (cos(length(gl_FragCoord.xy - center)*0.03)+1.0)/2.0;
	
	color = (color1+ color2)/2.0;

	float red	= (cos(PI*color/0.5+ttime*3.0)+1.0)/2.0;
	float green	= (sin(PI*color/0.5+ttime*3.0)+1.0)/2.0;
	float blue	= (sin(+ttime*3.0)+1.0)/2.0;
	
    gl_FragColor = vec4(red, green, blue, 1.0);
}