#pragma once
#include "glslprogram.h"
class SolidShader :
	public GLSLProgram
{
public:
	SolidShader(void);
	virtual ~SolidShader(void);
	virtual void setup(const glm::mat4 &model);
};

