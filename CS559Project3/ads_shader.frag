#version 400

layout (location = 0) out vec4 FragColor;

//lights
uniform vec3 light_position[5];
uniform vec4 lightColor[5];

//material properties
uniform vec3 ambientScale;
uniform vec3 diffuseScale;
uniform vec3 specularScale;
uniform float shininess;

uniform bool wireframe;
uniform bool flipUCoord;

flat in vec4 GColor;
in vec3 GPosition;
in vec3 GNormal;
noperspective in vec3 GEdgeDistance;

vec4 ads() {
	vec3 color = GColor.rgb;
	vec3 n = normalize(GNormal);
	vec3 return_color = vec3(0.0f);
	if (!gl_FrontFacing)
	n = -n;
	for(int i = 0; i < 5; i++) {
		if (lightColor[i].w != 0) {
			vec3 s = normalize(light_position[i] - GPosition);
			float s_dot_n = max(dot(s, n), 0.0);

			vec3 specular = vec3(0.0);
			vec3 v;
			vec3 r;

			v = normalize(-GPosition);
			r = reflect(-s, n);
			specular = (s_dot_n > 0 ? vec3(lightColor[i])*pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
			return_color +=
				ambientScale*lightColor[i].xyz*color +
				diffuseScale*lightColor[i].xyz*color*s_dot_n +
				specularScale*specular;
		}
	}

	return vec4(return_color, GColor.a);
}

void main() {
	float d = min(GEdgeDistance.x, GEdgeDistance.y);
	d = min(d, GEdgeDistance.z);
	float mixval = smoothstep(0,2,d);
	if(wireframe)
	{
	FragColor = mix(vec4(0.0f,1.0f,0.0f,1.0f), ads(), mixval);
	}
	else
	{
	FragColor = ads();
	}
}
