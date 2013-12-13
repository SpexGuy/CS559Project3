#pragma once
#include <glm/glm.hpp>
#include "Function.h"
#define _USE_MATH_DEFINES
#include <math.h>
class CameraDecorator;

/** 
 * A camera represents a positon and direction, and
 * is responsible for generating a view matrix.
 */
class Camera {
public:

	/* pushes the given Decorator onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual Camera *pushDecorator(CameraDecorator *d);

	/* stores the current top of the decorator stack in the bucket */
	virtual Camera *store(Camera *&bucket);
	
	/* generates a view matrix */
	virtual glm::mat4 generateViewMatrix() = 0;

	/* Once Controllers are implemented, tnese will be obselete */
	//@Deprecated
	void virtual moveForward(float offset) = 0;
	//@Deprecated
	void virtual moveRight(float offset) = 0;
	//@Deprecated
	void virtual moveUp(float offset) = 0;

	virtual ~Camera() {}

	//--------- Decorator Functions -----------------
	//The following functions exist only as wrappers to pushDecorator
	//to enrich the syntax

	/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	Camera *rotated(const glm::vec3 &axis, const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Camera *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Camera *scaled(const glm::vec3 &scale);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	Camera *animateRotation(const glm::vec3 &axis, TimeFunction<float> *angle);


};

/**
 * an abstract class which all decorators should extend
 *
 * The decorators form a stack on top of the underlying
 * Camera. This stack is upside-down so that the order
 * in which the decorators are activated is the same as
 * the order in which they were pushed onto the stack.
 *
 * Decorators manage their own memory, and will delete
 * child pointers all the way down the stack including
 * the terminating Camera.
 */
class CameraDecorator : public Camera {
protected:
	Camera *next;
	bool isTos;
public:
	CameraDecorator() :
		next(NULL),
		isTos(true)
	{}

	/* {@InheritDoc} */
	virtual Camera *pushDecorator(CameraDecorator *d);
	
	/* {@InheritDoc} */
	virtual Camera *store(Camera *&bucket);

	inline void setNext(Camera *next) {
		this->next = next;
	}
	inline Camera *getNext() {
		return next;
	}
	
	virtual ~CameraDecorator();

	//@Deprecated
	/* propagates the call down the stack */
	virtual void moveForward(float offset);

	//@Deprecated
	/* propagates the call down the stack */
	virtual void moveRight(float offset);

	//@Deprecated
	/* propagates the call down the stack */
	virtual void moveUp(float offset);

};

class FixedCamera : public Camera {
protected:
	glm::mat4 view;
public:
	FixedCamera();

	FixedCamera(const glm::mat4 &view) :
		view(view) {}

	FixedCamera(Camera *c) :
		view(c->generateViewMatrix()) {}

	inline virtual glm::mat4 generateViewMatrix() {
		return view;
	}
	
	//@Deprecated
	inline virtual void moveForward(float offset) {}
	//@Deprecated
	inline virtual void moveRight(float offset) {}
	//@Deprecated
	inline virtual void moveUp(float offset) {}
};

class BetterCamera : public Camera {
private:
	glm::ivec2 prevMouse;
	glm::vec2 rotation;
	glm::vec3 position;

public:

	BetterCamera(glm::vec3 pos) : position(pos) {rotation = glm::vec2(M_PI,M_PI/2.0f);} //very kludgy number.

	virtual glm::mat4 generateViewMatrix();
	
	inline void virtual moveForward(float offset) {}; //Stuck on an XY plane.
	
	inline void virtual moveRight(float offset) {position.x += offset;}
	
	inline void virtual moveUp(float offset) {position.y += offset;}

	void mouseUpdate(glm::ivec2 newMouse);

	void mouseClear(glm::ivec2 newMouse);
};