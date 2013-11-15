/** See Camera.h for architecture documentation */
#include "Camera.h"
#include "Rotatable.h"
#include "Translatable.h"
#include "Scaleable.h"
#include "Animation.h"

using namespace glm;


//---------------- Camera -----------------

Camera *Camera::pushDecorator(CameraDecorator *d) {
	d->setNext(this);
	return d;
}

Camera *Camera::store(Camera *&bucket) {
	bucket = this;
	return this;
}

Camera *Camera::rotated(const vec3 &axis, const float &angle) {
	return pushDecorator(new CamRotation(angle, axis));
}

Camera *Camera::translated(const vec3 &position) {
	return pushDecorator(new CamTranslation(position));
}

Camera *Camera::scaled(const vec3 &scale) {
	return pushDecorator(new CamScale(scale));
}

Camera *Camera::animateRotation(const vec3 &axis, TimeFunction<float> *angle) {
	return pushDecorator(new CamRotationAnimation(axis, angle));
}


//----------------- CameraDecorator -----------------

Camera *CameraDecorator::pushDecorator(CameraDecorator *dec) {
	Camera *d = next->pushDecorator(dec);
	if (d != next)
		isTos = false;
	next = d;
	return this;
}

Camera *CameraDecorator::store(Camera *&bucket) {
	if (isTos) {
		bucket = this;
	} else {
		next->store(bucket);
	}
	return this;
}

CameraDecorator::~CameraDecorator() {
	if (next != NULL)
		delete next;
}

void CameraDecorator::moveForward(float offset){
	next->moveForward(offset);
}

void CameraDecorator::moveRight(float offset){
	next->moveRight(offset);
}

void CameraDecorator::moveUp(float offset){
	next->moveUp(offset);
}

