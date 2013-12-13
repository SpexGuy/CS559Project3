#pragma once
#include "FrameBuffer.h"
#include "View.h"
#include <vector>

class FrameOperation {
public:
	void paint() {
		Graphics::inst()->drawRect2D(mat4(1.0f), vec2(-1), vec2(1));
	}

	virtual bool initialize() = 0;
	virtual void render(FrameBufferObject *fbo) = 0;
	virtual void takeDown() = 0;
	virtual ~FrameOperation() {}
};

class DrawModel : public FrameOperation, public View {
public:
	DrawModel(Projection *proj, 
			  Camera *cam,
			  Drawable *model,
			  Drawable *overlay) :
		View(proj, cam, model, overlay)
	{}

	virtual bool initialize();
	virtual void render(FrameBufferObject *fbo);
	virtual void takeDown();
};

class DrawPostProcess : public FrameOperation {
protected:
	int shaderName;
	GLSLProgram *shader;
public:
	DrawPostProcess(int shader) :
		shaderName(shader)
	{}

	virtual bool initialize();
	virtual void render(FrameBufferObject *fbo);
	virtual void takeDown();
};

class Frame {
private:
	Frame();
protected:
	FrameBufferObject *fbo;
	virtual void paint();
	std::vector<FrameOperation *> ops;

public:
	Frame(glm::ivec2 size) :
		ops(),
		fbo(new FrameBufferObject(size, 2))
	{}

	virtual inline bool initialize() {
		for (unsigned int c = 0; c < ops.size(); c++)
			if (!ops[c]->initialize())
				return false;

		return fbo->initialize();
	}

	virtual void render();
	virtual void drawToScreen();
	virtual inline void resize(int x, int y) {
		fbo->resize(x, y);
	}

	inline FrameBufferObject *getFBO() {
		return fbo;
	}

	virtual void takeDown() {
		for (unsigned int c = 0; c < ops.size(); c++)
			ops[c]->takeDown();

		fbo->takeDown();
	}

	virtual ~Frame() {
		for (unsigned int c = 0; c < ops.size(); c++)
			delete ops[c];

		delete fbo;
	}

	Frame *renderStuff(Projection *p, Camera *c, Drawable *m, Drawable *o) {
		ops.push_back(new DrawModel(p, c, m, o));
		return this;
	}

	Frame *postProcess(int shaderName) {
		ops.push_back(new DrawPostProcess(shaderName));
		return this;
	}
};




