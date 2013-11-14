#pragma once
#include "Drawable.h"
#include "Rotatable.h"

class SpheroidLight : public Drawable, public SphericalPositionMixin {
	virtual bool initialize();
	virtual void draw(const glm::mat4 &model);
	virtual void takeDown();
};