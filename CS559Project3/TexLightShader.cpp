#include "Graphics.h"
#include "TexLightShader.h"

using namespace glm;

TexLightShader::TexLightShader(void)
{
}

void TexLightShader::setup(const mat4 &model)
{
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
	this->setUniform("light_position", NUM_LIGHTS, g->getLightArray());
	this->setUniform("shininess", g->getShininess());
	this->setUniform("ambientScale", g->getAmbientVec());
	this->setUniform("diffuseScale", g->getDiffuseVec());
	this->setUniform("specularScale", g->getSpecularVec());
	this->setUniform("lightColor", NUM_LIGHTS, g->getLightColorArray());
	this->setUniform("flipUCoord", g->isFlipU());
	this->setUniform("wireframe", g->isWireframe());
	this->setUniform("ViewportMatrix", g->getViewportMatrix());
}

TexLightShader::~TexLightShader(void)
{
}
