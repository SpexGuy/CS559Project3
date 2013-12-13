#pragma once
#include "FrameBuffer.h"
#include "View.h"
#include <vector>

class Frame : public View {
private:
	Frame();
protected:
	FrameBufferObject *fbo;
	virtual void paint();
	std::vector<GLSLProgram *> ppos;
	std::vector<int> ppo_names;

public:
	Frame(glm::ivec2 size,
		  std::vector<int> ppo_names,
		  Projection *proj, 
		  Camera *cam,
		  Drawable *model,
		  Drawable *overlay) :
		View(proj, cam, model, overlay),
		ppo_names(ppo_names),
		fbo(new FrameBufferObject(size, ppo_names.size()+1))
	{}

	Frame(glm::ivec2 size,
		  Projection *proj, 
		  Camera *cam,
		  Drawable *model,
		  Drawable *overlay) :
		View(proj, cam, model, overlay),
		ppo_names(),
		ppos(),
		fbo(new FrameBufferObject(size))
	{}

	virtual inline bool initialize() {
		for (unsigned int c = 0; c < ppo_names.size(); c++)
			ppos.push_back(ShaderFlyweight::inst()->getShader(ppo_names[c]));

		return fbo->initialize();
	}

	virtual void render();
	virtual void drawToScreen();
	virtual inline void resize(int x, int y) {
		fbo->resize(x, y);
	}
	virtual void advance(GLSLProgram *prog);

	inline FrameBufferObject *getFBO() {
		return fbo;
	}

	virtual void takeDown() {
		fbo->takeDown();
	}

	virtual ~Frame() {
		delete fbo;
	}
};