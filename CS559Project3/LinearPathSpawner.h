#pragma once
#include "Drawable.h"
#include "Translatable.h"

class LinearPathSpawner {
private:
	LinearPathSpawner();
protected:
	float velocity;
	int minSpawnTime;
	int lastSpawnTime;
	float basePos;
	float probability;
	DrawableGroup *group;
	Drawable *prototype;

public:
	LinearPathSpawner(Drawable *prototype, float velocity,
		float width, float basePos, float probability,
		DrawableGroup *group) :
			prototype(prototype), velocity(velocity),
			probability(probability), group(group),
			minSpawnTime(int(ceil(width/velocity))),
			lastSpawnTime(0),
			basePos(basePos)
	{}
	
	virtual void update();
	virtual void spawn();

};

class BillboardLinearSpawner : public LinearPathSpawner {
public:
	BillboardLinearSpawner(Drawable *prototype, float velocity,
		float width, float basePos, float probability,
		DrawableGroup *group) :
	LinearPathSpawner(prototype, velocity, width, basePos, probability, group)
	{}

	virtual void spawn();
};

