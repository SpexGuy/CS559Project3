/* This class was based on an example by Perry Kivolowitz.
 */
#include "Shaders.h"
#include "Drawable.h"
#include "glslprogram.h"
#include "TexLightShader.h"
#include "SolidShader.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define BAD_GL_VALUE GLuint(-1)

ShaderFlyweight ShaderFlyweight::instance;
ShaderFlyweight *ShaderFlyweight::inst() {
	return &instance;
}

ShaderFlyweight::ShaderFlyweight() :
	loadedShaders() {
}

bool ShaderFlyweight::initialize() {

	//Here we make the glsl programs and then load them into the flyweight.
	GLSLProgram * texlightshader = new TexLightShader();
	texlightshader->compileShader("texture_shader.frag", GLSLShader::FRAGMENT);
	texlightshader->compileShader("texture_shader.vert", GLSLShader::VERTEX);
	texlightshader->compileShader("wireframe_texture.geom", GLSLShader::GEOMETRY);
	texlightshader->link();
	addShader(SHADER_TEXTURE, texlightshader);

	GLSLProgram * adsshader = new TexLightShader();
	adsshader->compileShader("ads_shader.frag", GLSLShader::FRAGMENT);
	adsshader->compileShader("ads_shader.vert", GLSLShader::VERTEX);
	adsshader->compileShader("wireframe_ads.geom", GLSLShader::GEOMETRY);
	adsshader->link();
	addShader(SHADER_ADS, adsshader);

	GLSLProgram * solidshader = new SolidShader();
	solidshader->compileShader("solid_shader.frag", GLSLShader::FRAGMENT);
	solidshader->compileShader("solid_shader.vert", GLSLShader::VERTEX);
	solidshader->compileShader("wireframe_solid.geom", GLSLShader::GEOMETRY);
	solidshader->link();
	addShader(SHADER_SOLID, solidshader);

	GLSLProgram * noiseshader = new TexLightShader();
	noiseshader->compileShader("noiseShader.frag", GLSLShader::FRAGMENT);
	noiseshader->compileShader("noiseShader.vert", GLSLShader::VERTEX);
	noiseshader->compileShader("wireframe_noise.geom", GLSLShader::GEOMETRY);
	noiseshader->link();
	addShader(SHADER_NOISE, noiseshader);

	GLSLProgram * pposhader = new PostProcessingShader();
	pposhader->compileShader("identity_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_IDENTITY, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("invert_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_INVERTED, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("simple_scanline_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_SCANLINE, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("static_noise_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_STATIC_NOISE, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("solar_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_SOLAR, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("sepia_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_SEPIA, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("plasma_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_PLASMA, pposhader);

	pposhader = new PostProcessingShader();
	pposhader->compileShader("border_ppo.frag", GLSLShader::FRAGMENT);
	pposhader->compileShader("ppo_shader.vert", GLSLShader::VERTEX);
	pposhader->link();
	addShader(PPO_BORDER, pposhader);

	return true;
}

void ShaderFlyweight::addShader(int name, GLSLProgram * shader) {
	assert(loadedShaders.find(name) == loadedShaders.end());
	loadedShaders[name] = shader;
}

GLSLProgram *ShaderFlyweight::getShader(int name) {
	assert(loadedShaders.find(name) != loadedShaders.end());
	return loadedShaders[name];
}

void ShaderFlyweight::takeDown() {
	for (map<int, GLSLProgram *>::iterator
				iter = loadedShaders.begin(),
				end = loadedShaders.end();
			iter != end;
			iter++)
	{
		delete iter->second;
	}
	loadedShaders.clear();
}

ShaderFlyweight::~ShaderFlyweight() {
	assert(loadedShaders.size() == 0);
}





void PostProcessingShader::setup(const glm::mat4 &model) {
	setUniform("size", vec2(Graphics::inst()->getSize()));
	setUniform("time", float(Graphics::inst()->getTime())/1000.0f);
}
