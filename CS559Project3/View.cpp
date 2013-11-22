#include <iostream>
#include "View.h"
#include "Camera.h"
#include "Drawable.h"
#include "Graphics.h"
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;


View::View(Projection *p, Camera *c, Drawable *model, Drawable *overlay) :
	proj(p), camera(c), model(model), overlay(overlay) {}

void View::render() {
	this->setupCamera();
	mat4 context = mat4(1.0f);

	glEnable(GL_DEPTH_TEST);
	model->draw(context);

	glDisable(GL_DEPTH_TEST);
	overlay->draw(context);
}

void View::setupCamera() {
	Graphics::inst()->setProjection(proj->generateProjectionMatrix());
	Graphics::inst()->setView(camera->generateViewMatrix());
}

void ViewOverlay::setupCamera() const {
	Graphics::inst()->setProjection(
			ortho(0.0f, float(Graphics::inst()->getSize().x),
				  0.0f, float(Graphics::inst()->getSize().y),
				  0.0f, 1.0f));
	Graphics::inst()->setView(
			lookAt(vec3(0.0f, 0.0f, 0.0f),
				   vec3(0.0f, 0.0f, -1.0f),
				   vec3(0.0f, 1.0f, 0.0f)));
}



bool HudOverlay::draw(const glm::mat4 &model)
{
	setupCamera();
	Graphics::inst()->setColor(vec4(1.0f,1.0f,1.0f,1.0f));
	float lines = (float)text.size();
	float size = 10.0f;

	for(uint i = 0; i < text.size(); i++) {
		Graphics::inst()->drawText2D(model, 5.0f, 2.1f*size*(float)(i) + 10.0f , text[i], size);
	}

	return true;
}
