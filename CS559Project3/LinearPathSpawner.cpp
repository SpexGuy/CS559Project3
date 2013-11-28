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