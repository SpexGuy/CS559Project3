#pragma once
#include "FrameBuffer.h"
#include "View.h"

class Frame : public View {
private:
	Frame();
protected:
	FrameBufferObject *fbo;
public:
	Frame(FrameBufferObject *fbo, Projection *proj, Camera *cam, Drawable *model, Drawable *overlay) :
		View(proj, cam, model, overlay), fbo(fbo) {}

	virtual void render();

	virtual ~Frame() {}
};