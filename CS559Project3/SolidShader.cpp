#include "SolidShader.h"
#include "Graphics.h"
#include "ErrorCheck.h"

SolidShader::SolidShader(void)
{
}

void SolidShader::setup(const mat4 &model)
{
	checkError("Before SolidShader Setup");
	/*
	Our uniforms are:
	mvp/modelview/normal matrices, and textureIndex and the light info.
	*/
	Graphics *g = Graphics::inst();
	mat4 mv = g->getModelview(model);
	mat4 mvp = g->getProjection() * mv;
	this->setUniform("mvp", mvp);
	this->setUniform("color", g->getColor());
	this->setUniform("wireframe", g->isWireframe());
	this->setUniform("ViewportMatrix", g->getViewportMatrix());
	checkError("After SolidShader Setup");
}

SolidShader::~SolidShader(void)
{
}
