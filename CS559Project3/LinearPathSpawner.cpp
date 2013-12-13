#include "LinearPathSpawner.h"


void LinearPathSpawner::update() {
	int time = Graphics::inst()->getTime();
	if (time > lastSpawnTime + minSpawnTime &&
			float(rand())/RAND_MAX <= probability) {
		spawn();
	}
}

void LinearPathSpawner::spawn() {
	int time = Graphics::inst()->getTime();
	lastSpawnTime = time;
	group->addElement(
		prototype
			->copyStack()
			->animateTranslation(
				new Vec3TimeFunction(
					new ConstantTimeFunction(0.0f),
					new ConstantTimeFunction(0.0f),
					new LinearTimeFunction(velocity, basePos-velocity*time)))
			->obselesceOffscreen()
			->breakDelete());
}

void BillboardLinearSpawner::spawn() {
	int time = Graphics::inst()->getTime();
	lastSpawnTime = time;
	group->addElement(
		prototype
			->copyStack()
			->animateTranslation(
				new Vec3TimeFunction(
					new ConstantTimeFunction(0.0f),
					new ConstantTimeFunction(0.0f),
					new LinearTimeFunction(velocity, basePos-velocity*time)))
			->billboard(vec3(0.0f, 1.0f, 0.0f))
			->obselesceOffscreen()
			->breakDelete());
}