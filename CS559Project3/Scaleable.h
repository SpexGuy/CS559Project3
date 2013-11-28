#pragma once
#include "Drawable.h"
#include "Camera.h"
#include <glm/glm.hpp>



class Scale : public DrawableDecorator {
protected:
	glm::vec3 scaleVec;
public:
	Scale() :
		scaleVec(1.0f) {}
	Scale(const glm::vec3 &scale) :
		scaleVec(scale) {}

	virtual bool draw(const glm::mat4 &model);
	virtual Drawable *copyStack();
	
	inline void setScale(const glm::vec3 &scale) {
		this->scaleVec = scale;
	}
	inline glm::vec3 getScale() const {
		return scaleVec;
	}
};

class CamScale : public CameraDecorator {
protected:
	glm::vec3 scaleVec;
public:
	CamScale() :
		scaleVec(1.0f) {}
	CamScale(const glm::vec3 &scale) :
		scaleVec(scale) {}

	virtual glm::mat4 generateViewMatrix();

	inline void setScale(const glm::vec3 &scale) {
		this->scaleVec = scale;
	}
	inline glm::vec3 getScale() const {
		return scaleVec;
	}
};

