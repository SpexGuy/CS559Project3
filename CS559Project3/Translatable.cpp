#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Translatable.h"

using namespace glm;


void Translation::draw(const mat4 &model) {
	child->draw(translate(model, pos));
}



mat4 CamTranslation::generateViewMatrix() {
	return translate(next->generateViewMatrix(), -pos);
}
