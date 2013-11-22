#pragma once
#include "glslprogram.h"
class SolidShader :
	public GLSLProgram
{
public:
	SolidShader(void);
	virtual ~SolidShader(void);
	virtual void bind(const glm::mat4 &model);
};

