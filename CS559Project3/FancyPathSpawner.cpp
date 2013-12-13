#include "FancyPathSpawner.h"
#define _USE_MATH_DEFINES
#include <math.h>


void FancyPathSpawner::spawn() {
	int time = Graphics::inst()->getTime();
	lastSpawnTime = time;
	//set up the direction of the new thing!
	vec3 direction = glm::normalize(vec3(
		(float(rand())/float(RAND_MAX)*1.0f) - 0.5f,
		(float(rand())/float(RAND_MAX)*1.0f) - 0.5f,
		1.0f));

	//set up the spawnpoint of the original thing!
	float radius = float(rand())/float(RAND_MAX) * spawnRadius;
	float angle = float(rand())/float(RAND_MAX) * 2.0f * float(M_PI);
	vec3 spawnpos = vec3(radius * cos(angle), radius * sin(angle), -2.0f);
	Drawable *stack;	
	group->addElement(
		stack = prototype
			->copyStack()
			->translated(spawnpos)
			->animateTranslation(
				new Vec3TimeFunction(
					new LinearTimeFunction(velocity*direction.x, spawnpos.x-velocity*direction.x*time),
					new LinearTimeFunction(velocity*direction.y, spawnpos.y-velocity*direction.y*time),
					new LinearTimeFunction(velocity*direction.z, spawnpos.z-velocity*direction.z*time)))
			->obselesceOffscreen()
			->drawZOrdered()
			->setGlBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
			->inRandomColor(vec2(0.2f, 0.5f))
			->inMaterial(0.2f, 1.0f, 100)
			->useShader(SHADER_ADS)
			->breakDelete());
	stack->initialize();
}