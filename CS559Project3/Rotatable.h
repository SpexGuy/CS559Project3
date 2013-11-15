#pragma once
#include <glm/glm.hpp>
#include "Drawable.h"
#include "Camera.h"
#include "Function.h"

/**
 * A mixin class to facilitate implementation of the Rotatable interface
 */
//@Deprecated

class Rotation : public DrawableDecorator {
private:
	Rotation();
protected:
	float angle;
	glm::vec3 axis;
public:
	Rotation(float angle, const glm::vec3 &axis) :
		angle(angle), axis(axis) {}

	virtual void draw(const glm::mat4 &model);

	inline void setAngle(float angle) {
		this->angle = angle;
	}
	inline void setAxis(const glm::vec3 &axis) {
		this->axis = axis;
	}
};

class RotationAnimation : public DrawableDecorator {
private:
	RotationAnimation();
protected:
	glm::vec3 axis;
	TimeFunction<float> *angle;
public:
	RotationAnimation(const glm::vec3 &axis, TimeFunction<float> *angle) :
		axis(axis), angle(angle) {}

	virtual void draw(const glm::mat4 &model);
};

class CamRotation : public CameraDecorator {
private:
	CamRotation();
protected:
	float angle;
	glm::vec3 axis;
public:
	CamRotation(float angle, const glm::vec3 &axis) :
		angle(angle), axis(axis) {}

	virtual glm::mat4 generateViewMatrix();

	inline void setAngle(float angle) {
		this->angle = angle;
	}
	inline void setAxis(const glm::vec3 &axis) {
		this->axis = axis;
	}
};

class CamRotationAnimation : public CameraDecorator {
private:
	CamRotationAnimation();
protected:
	glm::vec3 axis;
	TimeFunction<float> *angle;
public:
	CamRotationAnimation(const glm::vec3 &axis, TimeFunction<float> *angle) :
		axis(axis), angle(angle) {}

	virtual glm::mat4 generateViewMatrix();
};

/**
 * A mixin which gives data members and modifying functions
 * to represent a spherical coordinate system
 */
//@Deprecated
class SphericalAngleMixin {
protected:
	float angle;
	float axisAngle;
public:
	inline SphericalAngleMixin() {
		this->angle = 0.0f;
		this->axisAngle = 90.0f;
	}

	inline void setAngle(float angle) {
		this->angle = float(fmod(angle, 360.0f));
	}
	inline void addAngle(float difference) {
		setAngle(angle+difference);
	}
	
	/* axisAngle will be clamped into the range [1, 179],
	 * with 1 being close to parallel to the axis
	  * and 90 being perpendicular */
	inline void setAxisAngle(float axisAngle) {
		if (axisAngle < 1)
			axisAngle = 1.0f;
		if (axisAngle > 179)
			axisAngle = 179.0f;
		this->axisAngle = axisAngle;
	}
	inline void addAxisAngle(float difference) {
		setAxisAngle(axisAngle+difference);
	}
};

//@Deprecated
class SphericalCoordinateMixin : public SphericalAngleMixin {
protected:
	float radius;
public:
	inline SphericalCoordinateMixin() {
		this->radius = 1;
	}
	inline void setRadius(float radius) {
		if (radius <= 0)
			radius = 0.0000001f;
		this->radius = radius;
	}
	inline void addRadius(float difference) {
		setRadius(radius+difference);
	}
};

/**
 * A Transformer mixin which implements rotation using
 * a spherical coordinate system
 */
//@Deprecated
class SphericalRotationMixin : public SphericalAngleMixin {
protected:
	virtual void transform(glm::mat4 & context) const;

};

//@Deprecated
class SphericalPositionMixin : public SphericalCoordinateMixin {
protected:
	virtual void transform(glm::mat4 & context) const;
};