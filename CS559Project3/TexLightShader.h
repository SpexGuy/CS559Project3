#pragma once
#include "glslprogram.h"
class TexLightShader :
	public GLSLProgram
{
public:
	TexLightShader(void);
	virtual ~TexLightShader(void);
	virtual void bind(const glm::mat4 &model);
};

