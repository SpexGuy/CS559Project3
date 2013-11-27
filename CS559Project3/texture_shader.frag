#version 400

layout (location = 0) out vec4 FragColor;

uniform sampler2D textureIndex;

uniform vec3 light_position[5];

//The lower the bigger the shine
uniform float shininess;

//these shoud add to (1, 1, 1, 1)
uniform vec3 ambientScale;
uniform vec3 diffuseScale;
uniform vec4 specularColor[5];

uniform bool wireframe;

noperspective in vec3 GEdgeDistance;
in vec3 GPosition;
in vec3 GNormal;
in vec2 GUV;

vec4 ads() {
  vec3 color = texture( textureIndex, GUV ).rgb;
  vec3 n = normalize(GNormal);
  vec4 return_color;
  if (!gl_FrontFacing)
	n = -n;
  for(int i = 0; i < 5; i++)
  {
  vec3 s = normalize(light_position[i] - GPosition);
  float s_dot_n = max(dot(s, n), 0.0);

  vec3 specular = vec3(0.0);
  vec3 v;
  vec3 r;
  if (specularColor[i].w != 0) {
	v = normalize(-GPosition);
	r = reflect(-s, n);
	specular = (s_dot_n > 0 ? vec3(specularColor[i])*pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
  }

  return_color += vec4(
	ambientScale*color +
	diffuseScale*color*s_dot_n +
	specular,
	1.0);
  }

  return return_color;
}

void main(){
    // Output color = color of the texture at the specified UV
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