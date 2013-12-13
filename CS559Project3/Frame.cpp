#include "Frame.h"

using namespace glm;

void Frame::render() {
	for (uint c = 0; c < ops.size(); c++) {
		ops[c]->render(fbo);
	}
}

void Frame::paint() {
	Graphics::inst()->drawRect2D(mat4(1.0f), vec2(-1), vec2(1));
}

void Frame::drawToScreen() {
	static GLSLProgram *texShader = ShaderFlyweight::inst()->getShader(PPO_IDENTITY);
	Graphics::inst()->setShader(texShader);
	fbo->bindTexture();
	paint();
}



void ClearColorBuffer::render(FrameBufferObject *fbo) {
	fbo->bindDraw();
	glClear(GL_COLOR_BUFFER_BIT);
	fbo->unbindDraw();
}




bool DrawModel::initialize() {
	return true;
}

void DrawModel::render(FrameBufferObject *fbo) {
	fbo->bindDraw();
	glClear(GL_DEPTH_BUFFER_BIT);
	View::render();
	fbo->unbindDraw();
}

void DrawModel::takeDown() {

}




bool DrawPostProcess::initialize() {
	shader = ShaderFlyweight::inst()->getShader(shaderName);
	return true;
}

void DrawPostProcess::render(FrameBufferObject *fbo) {
	Graphics::inst()->setShader(shader);
	fbo->bindTexture();
	fbo->incrementBuffer();
	fbo->bindDraw();
	paint();
	fbo->unbindDraw();
}

void DrawPostProcess::takeDown() {

}


void ChangeBuffer::render(FrameBufferObject *fbo) {
	Graphics::inst()->setShader(shader);
	fbo->bindTexture();
	other->bindDraw();
	paint();
	other->unbindDraw();
}


