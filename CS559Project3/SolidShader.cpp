#include "SolidShader.h"
#include "Graphics.h"

SolidShader::SolidShader(void)
{
}

void SolidShader::bind(const mat4 &model)
{
	/*
	What to do here!
	Get state from Graphics and load into the uniforms!
	Equip/use the shader.
	*/
	use();
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
}

SolidShader::~SolidShader(void)
{
}
