#include "Scaleable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;



void Scale::draw(const mat4 &model) {
	child->draw(scale(model, scaleVec));
}



mat4 CamScale::generateViewMatrix() {
	return scale(next->generateViewMatrix(),
		vec3(1.0f/scaleVec.x, 1.0f/scaleVec.y, 1.0f/scaleVec.z));
}
