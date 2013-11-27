#pragma once
#include "Drawable.h"
#include "Rotatable.h"

class SpheroidLight : public Drawable, public SphericalPositionMixin {
public:
	SpheroidLight(int index, vec4 color);
	virtual bool initialize();
	virtual bool draw(const glm::mat4 &model);
	virtual void takeDown();
private:
	int index;
	vec4 color;
	SpheroidLight();
};