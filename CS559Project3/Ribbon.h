#pragma once
#include "Drawable.h"
#include "Spline.h"
#include "Mesh.h"
#include <vector>

class RibbonBuilder {
protected:
	Drawable *ribbonStack;
	DrawableGroup *ribbons;
	float segmentLength;
	int segmentTime;
	int lastSpawnTime;

	SplinePoint3 lastRibbonPoint;
	SplinePoint1 lastRibbonAngle;
	glm::vec3 arb;
	glm::vec3 absolute;
	glm::vec3 lastPoint;

	Texture *texture;
	int resolution;
	float width;

	virtual Mesh *makeRibbonSegment(
				const SplinePoint3 &after,
				const SplinePoint1 &afterAngle);

	virtual SplinePoint3 generatePoint();
	virtual SplinePoint1 generateAngle();
public:
	RibbonBuilder(DrawableGroup *container,
					glm::vec3 position,
					float velocity,
					float length,
					float width,
					int resolution,
					glm::vec3 arbitrary,
					glm::vec3 startAbsolute,
					Texture *texture = NULL);

	virtual bool initialize();

	virtual void update();

	virtual void spawn(int time);

	virtual ~RibbonBuilder() {
		//ribbons->deleteAll();
		delete ribbonStack;
	}
};
