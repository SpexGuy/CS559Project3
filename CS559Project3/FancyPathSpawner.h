#pragma once
#include "linearpathspawner.h"
class FancyPathSpawner :
	public LinearPathSpawner
{
private:
	FancyPathSpawner();
protected:
	float spawnRadius;
public:
		FancyPathSpawner(Drawable *prototype, float velocity,
		float width, float basePos, float probability,
		DrawableGroup *group, float spawnRadius) :
			LinearPathSpawner(prototype, velocity, width, basePos, probability, group),
			spawnRadius(spawnRadius)
	{}

	virtual void spawn();
};

