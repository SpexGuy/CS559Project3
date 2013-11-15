#pragma once
#include <glm/glm.hpp>
#include "Drawable.h"
#include "Camera.h"


class Translation : public DrawableDecorator {
protected:
	glm::vec3 pos;
public:
	Translation() :
		pos(0.0f) {}
	Translation(const glm::vec3 &pos) :
		pos(pos) {}

	virtual void draw(const glm::mat4 &model);

	inline glm::vec3 getPosition() {
		return pos;
	}
	inline void setPosition(const glm::vec3 &pos) {
		this->pos = pos;
	}
};

class CamTranslation : public CameraDecorator {
protected:
	glm::vec3 pos;
public:
	CamTranslation() :
		pos(0.0f) {}
	CamTranslation(const glm::vec3 &pos) :
		pos(pos) {}

	virtual glm::mat4 generateViewMatrix();

	inline glm::vec3 getPosition() {
		return pos;
	}
	inline void setPosition(const glm::vec3 &pos) {
		this->pos = pos;
	}
};
