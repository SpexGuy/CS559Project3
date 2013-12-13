/** See Camera.h for architecture documentation */
#include "Camera.h"
#include "Rotatable.h"
#include "Translatable.h"
#include "Scaleable.h"
#include "Animation.h"
#define _USE_MATH_DEFINES
#include <math.h>

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


//---------------- FixedCamera ---------------

FixedCamera::FixedCamera() :
	view(lookAt(vec3(0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 1.0f, 0.0f)))
{}

glm::mat4 BetterCamera::generateViewMatrix() {
	vec3 eyeDelta = vec3(
		(sin(rotation.y) * sin(rotation.x)),
		(cos(rotation.y)),
		(sin(rotation.y) * cos(rotation.x))
		);
	return lookAt(position, position + eyeDelta, vec3(0.0f,1.0f,0.0f)); 
}

//change the position on the mouse delta
void BetterCamera::mouseUpdate(ivec2 newMouse){
	ivec2 delta = prevMouse - newMouse;
	vec2 rotationdelta = vec2(delta) * 0.25f * (float(M_PI)/180.0f); //mess with this magic number???
	rotation += rotationdelta;
	//update the previous mouse position!
	prevMouse = newMouse;
}

//reset the 'old' mouse position on click to avoid jumpiness
void BetterCamera::mouseClear(ivec2 newMouse){
	prevMouse = newMouse;
}
