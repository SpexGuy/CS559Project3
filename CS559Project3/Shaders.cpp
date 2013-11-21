/* This class was based on an example by Perry Kivolowitz.
 */
#include "Shaders.h"
#include "Drawable.h"
#include "glslprogram.h"
#include "TexLightShader.h"
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
	texlightshader->link();
	addShader(SHADER_TEXTURE, texlightshader);

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
