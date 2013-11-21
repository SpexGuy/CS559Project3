#include "Graphics.h"
#include "TexLightShader.h"

using namespace glm;

TexLightShader::TexLightShader(void)
{
}

void TexLightShader::bind(const mat4 &model)
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
	mat3 nm = g->getNormalMatrix(mv);
	this->setUniform("mvp", mvp);
	this->setUniform("modelview_matrix", mv);
	this->setUniform("normal_matrix", nm);
	this->setUniform("textureIndex", g->getTexture());
	this->setUniform("light_position", g->getLightPos());
	this->setUniform("shininess", g->getShininess());
	this->setUniform("ambientScale", g->getAmbientVec());
	this->setUniform("diffuseScale", g->getDiffuseVec());
	this->setUniform("specularColor", g->getSpecularColor());





}

TexLightShader::~TexLightShader(void)
{
}
