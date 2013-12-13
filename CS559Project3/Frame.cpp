#include "Frame.h"

using namespace glm;

void Frame::render() {
	fbo->resetBufferIndex();
	fbo->bindDraw();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	View::render();
	fbo->unbindDraw();
	for (uint c = 0; c < ppos.size(); c++) {
		advance(ppos[c]);
	}
}

void Frame::advance(GLSLProgram *s) {
	Graphics::inst()->setShader(s);
	fbo->bindTexture();
	fbo->incrementBuffer();
	fbo->bindDraw();
	paint();
	fbo->unbindDraw();
}

void Frame::paint() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Graphics::inst()->setProjection(
		ortho(0.0f,1.0f,0.0f,1.0f,0.0f,1.0f));
	Graphics::inst()->setView(mat4(1.0f));
	Graphics::inst()->drawRect2D(mat4(1.0f), vec2(-1), vec2(1));
}

void Frame::drawToScreen() {
	static GLSLProgram *texShader = ShaderFlyweight::inst()->getShader(PPO_IDENTITY);
	Graphics::inst()->setShader(texShader);
	fbo->bindTexture();
	paint();
}


