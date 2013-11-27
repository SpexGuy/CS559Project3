#include "Frame.h"

void Frame::render() {
	fbo->bindDraw();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	View::render();
	fbo->unbindDraw();
}


