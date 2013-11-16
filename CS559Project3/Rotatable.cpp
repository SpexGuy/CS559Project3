#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Rotatable.h"
#include <iostream>

using namespace glm;
using namespace std;



bool Rotation::draw(const mat4 &model) {
	return child->draw(rotate(model, angle, axis));
}



bool RotationAnimation::draw(const mat4 &model) {
	return child->draw(rotate(model, angle->evaluate(Graphics::inst()->getTime()), axis));
}



mat4 CamRotation::generateViewMatrix() {
	return rotate(next->generateViewMatrix(), -angle, axis);
}



mat4 CamRotationAnimation::generateViewMatrix() {
	return rotate(next->generateViewMatrix(),
			-(angle->evaluate(Graphics::inst()->getTime())),
			axis);
}



void SphericalRotationMixin::transform(mat4 &context) const {
	context = glm::rotate(context, angle, vec3(0.0f, 1.0f, 0.0f));
	//since the context has been rotated, (1, 0, 0) is now
	//perpendicular to the plane on which we want to rotate
	context = glm::rotate(context, axisAngle, vec3(1.0f, 0.0f, 0.0f));
}

void SphericalPositionMixin::transform(mat4 &context) const {
	context = glm::rotate(context, angle, vec3(0.0f, 1.0f, 0.0f));
	context = glm::rotate(context, axisAngle, vec3(1.0f, 0.0f, 0.0f));
	context = glm::translate(context, vec3(0.0f, radius, 0.0f));
}