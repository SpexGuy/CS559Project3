#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Translatable.h"

using namespace glm;


bool Translation::draw(const mat4 &model) {
	return child->draw(translate(model, pos));
}
Drawable *Translation::copyStack() {
	Translation *copy = new Translation(*this);
	copy->setChild(child->copyStack());
	return copy;
}


bool TranslationAnimation::draw(const mat4 &model) {
	return child->draw(translate(model, pos->evaluate(Graphics::inst()->getTime())));
}
Drawable *TranslationAnimation::copyStack() {
	TranslationAnimation *copy = new TranslationAnimation(*this);
	copy->setChild(child->copyStack());
	return copy;
}



mat4 CamTranslation::generateViewMatrix() {
	return translate(next->generateViewMatrix(), -pos);
}
