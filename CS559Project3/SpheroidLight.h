#pragma once
#include "Drawable.h"
#include "Rotatable.h"

class SpheroidLight : public Drawable, public SphericalPositionMixin {
public:
	virtual bool initialize();
	virtual bool draw(const glm::mat4 &model);
	virtual void takeDown();

};