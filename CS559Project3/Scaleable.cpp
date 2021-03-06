#include "Scaleable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;



bool Scale::draw(const mat4 &model) {
	return child->draw(scale(model, scaleVec));
}
Drawable *Scale::copyStack() {
	Scale *copy = new Scale(*this);
	copy->setChild(child->copyStack());
	return copy;
}



mat4 CamScale::generateViewMatrix() {
	return scale(next->generateViewMatrix(),
		vec3(1.0f/scaleVec.x, 1.0f/scaleVec.y, 1.0f/scaleVec.z));
}
