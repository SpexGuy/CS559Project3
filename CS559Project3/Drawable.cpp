/** See Drawable.h for architecture documentation */
#include <list>
#include "Drawable.h"
#include "Graphics.h"
#include "Rotatable.h"
#include "Scaleable.h"
#include "Translatable.h"
#include "Animation.h"
#include "ErrorCheck.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;


//--------------- Drawable --------------
Drawable *Drawable::pushDecorator(DrawableDecorator *d) {
	d->setChild(this);
	return d;
}

Drawable *Drawable::store(Drawable *&bucket) {
	bucket = this;
	return this;
}

Drawable *Drawable::copyStack() {
	return this;
}

//		----- Decorator Convenience Functions ------
Drawable *Drawable::rotated(const vec3 &axis, const float &angle) {
	return pushDecorator(new Rotation(angle, axis));
}

Drawable *Drawable::translated(const vec3 &position) {
	return pushDecorator(new Translation(position));
}

Drawable *Drawable::scaled(const vec3 &scale) {
	return pushDecorator(new Scale(scale));
}

Drawable *Drawable::animateRotation(const vec3 &axis, TimeFunction<float> *angle) {
	return pushDecorator(new RotationAnimation(axis, angle));
}

Drawable *Drawable::animateTranslation(TimeFunction<vec3> *pos) {
	return pushDecorator(new TranslationAnimation(pos));
}

Drawable *Drawable::disableDepthTest() {
	return pushDecorator(new DisableDepthTest());
}

Drawable *Drawable::disableDepthMask() {
	return pushDecorator(new DisableDepthMask());
}

Drawable *Drawable::disableCullFace() {
	return pushDecorator(new DisableCullFace());
}

Drawable *Drawable::inColor(vec4 color) {
	return pushDecorator(new Color(color));
}

Drawable *Drawable::inMaterial(const float &a, const float &s, const float &shiny) {
	return pushDecorator(new Material(a, s, shiny));
}

Drawable *Drawable::resetColor() {
	return pushDecorator(new ColorReset());
}

Drawable *Drawable::resetMaterial() {
	return pushDecorator(new MaterialReset());
}

Drawable *Drawable::billboard(const vec3 &axis) {
	return pushDecorator(new BillboardTransform(axis));
}

Drawable *Drawable::useMVMode(int mode) {
	return pushDecorator(new ModelviewMode(mode));
}

Drawable *Drawable::obselesceOffscreen() {
	return pushDecorator(new OffscreenObselescence());
}

Drawable *Drawable::breakDelete() {
	return pushDecorator(new NoDeletion());
}

Drawable *Drawable::useShader(int name) {
	return pushDecorator(new ShaderUse(name));
}

Drawable *Drawable::setGlBlendFunc(GLenum sfactor, GLenum dfactor) {
	return pushDecorator(new SetGlBlendFunc(sfactor, dfactor));
}

Drawable *Drawable::drawZOrdered() {
	return pushDecorator(new DrawZOrdered());
}

Drawable *Drawable::inRandomColor(const vec2 alpha,
								  const vec2 red,
								  const vec2 green,
								  const vec2 blue) {
	return pushDecorator(new InRandomColor(alpha, red, green, blue));
}





//---------------- DrawableDecorator ---------------

Drawable *DrawableDecorator::pushDecorator(DrawableDecorator *dec) {
	Drawable *d = child->pushDecorator(dec);
	if (d != child) {
		isTos = false;
		child = d;
	}
	return this;
}

Drawable *DrawableDecorator::store(Drawable *&bucket) {
	if (isTos) {
		bucket = this;
	} else {
		child->store(bucket);
	}
	return this;
}

bool DrawableDecorator::initialize() {
	return child->initialize();
}

bool DrawableDecorator::draw(const mat4 &model) {
	return child->draw(model);
}

void DrawableDecorator::takeDown() {
	child->takeDown();
}

DrawableDecorator::~DrawableDecorator() {
	if (child != NULL)
		delete child;
}


//----------------- DrawableGroup ------------------

DrawableGroup::DrawableGroup() {
	elements = list<Drawable*>();
}

bool DrawableGroup::draw(const mat4 &model) {
	for (
		list<Drawable *>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;)
	{
		if (!(*iterator)->draw(model)) {
			(*iterator)->takeDown();
			delete *iterator;
			iterator = elements.erase(iterator);
		} else {
			++iterator;
		}
	}
	return true;
}

bool DrawableGroup::initialize() {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		if (!(*iterator)->initialize())
			return false;
	}
	return true;
}

void DrawableGroup::takeDown() {
	for (
		list<Drawable*>::const_iterator
			iterator = elements.begin(),
			end = elements.end();
		iterator != end;
		++iterator)
	{
		(*iterator)->takeDown();
	}
}

void DrawableGroup::addLight(Drawable *light) {
	elements.push_front(light);
}

void DrawableGroup::addElement(Drawable *el) {
	elements.push_back(el);
}

void DrawableGroup::clearElements() {
	elements.clear();
}

list<Drawable *> *DrawableGroup::getElements() {
	return &elements;
}


//------------ Decorator Implementations ---------------



void InRandomColor::setColor() {
	thisColor = vec4(0.0f);
	thisColor.r = (float(rand())/float(RAND_MAX) * (red.y - red.x) + red.x);
	thisColor.g = (float(rand())/float(RAND_MAX) * (green.y - green.x) + green.x);
	thisColor.b = (float(rand())/float(RAND_MAX) * (blue.y - blue.x) + blue.x);
	thisColor.a = (float(rand())/float(RAND_MAX) * (alpha.y - alpha.x) + alpha.x);
}

bool InRandomColor::draw(const glm::mat4 &model) {
	Graphics::inst()->setColor(thisColor);
	bool ret = child->draw(model);
	return ret;
}

Drawable *InRandomColor::copyStack() {
	InRandomColor *copy = new InRandomColor(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool DrawZOrdered::draw(const mat4 &model) {
	Graphics::inst()->addZDrawable(child, model);
	return true;
}

Drawable *DrawZOrdered::copyStack() {
	DrawZOrdered *copy = new DrawZOrdered(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool DisableDepthTest::draw(const mat4 &model) {
	glDisable(GL_DEPTH_TEST);
	bool ret = child->draw(model);
	glEnable(GL_DEPTH_TEST);
	return ret;
}
Drawable *DisableDepthTest::copyStack() {
	DisableDepthTest *copy = new DisableDepthTest(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool SetGlBlendFunc::draw(const mat4 &model) {
	glEnable(GL_BLEND);
	glBlendFunc(this->sfactor, this->dfactor);
	bool ret = child->draw(model);
	glBlendFunc(GL_ONE,GL_ZERO); //this is the default value for glBlendFunc
	glDisable(GL_BLEND);
	return ret;
}
Drawable *SetGlBlendFunc::copyStack() {
	SetGlBlendFunc *copy = new SetGlBlendFunc(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool DisableDepthMask::draw(const mat4 &model) {
	glDepthMask(GL_FALSE);
	bool ret = child->draw(model);
	glDepthMask(GL_TRUE);
	return ret;
}
Drawable *DisableDepthMask::copyStack() {
	DisableDepthMask *copy = new DisableDepthMask(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool DisableCullFace::draw(const mat4 &model) {
	glDisable(GL_CULL_FACE);
	bool ret = child->draw(model);
	glEnable(GL_CULL_FACE);
	return ret;
}
Drawable *DisableCullFace::copyStack() {
	DisableCullFace *copy = new DisableCullFace(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool Color::draw(const mat4 &model) {
	Graphics::inst()->setColor(color);
	return child->draw(model);
}
Drawable *Color::copyStack() {
	Color *copy = new Color(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool Material::draw(const mat4 &model) {
	Graphics::inst()->setMaterial(ambient, specular, shininess);
	return child->draw(model);
}
Drawable *Material::copyStack() {
	Material *copy = new Material(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool ColorReset::draw(const mat4 &model) {
	//save the old color
	vec4 color = Graphics::inst()->getColor();
	//continue down the stack
	bool ret = child->draw(model);
	//restore the old color
	Graphics::inst()->setColor(color);
	return ret;
}
Drawable *ColorReset::copyStack() {
	ColorReset *copy = new ColorReset(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool MaterialReset::draw(const mat4 &model) {
	//save the old material
	float a = Graphics::inst()->getAmbient();
	float s = Graphics::inst()->getSpecular();
	float shiny = Graphics::inst()->getShininess();
	//continue down the stack
	bool ret = child->draw(model);
	//restore the old material
	Graphics::inst()->setMaterial(a, s, shiny);
	return ret;
}
Drawable *MaterialReset::copyStack() {
	MaterialReset *copy = new MaterialReset(*this);
	copy->setChild(child->copyStack());
	return copy;
}

/** this algorithm is adapted from
 * http://nehe.gamedev.net/article/billboarding_how_to/18011/
 */
bool BillboardTransform::draw(const mat4 &model) {
	checkError("Before Billboard Transform");
	//convert camera position to local coordinate space
	vec4 cameraPos = inverse(Graphics::inst()->getView() * model) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//find the appropriate orthogonal basis
	vec3 look = normalize(vec3(cameraPos));	//calculate the look vector
	vec3 right = cross(vec3(axis), look);	//right = up x look
	if (right != vec3(0.0f))				//handle special case: up = look
		right = normalize(right);
	look = cross(right, axis);				//calculate the orthogonal look vector

	//build a transformation matrix
	mat4 transform;
	transform[0] = vec4(     right      , 0.0f);
	transform[1] = vec4(      axis      , 0.0f);
	transform[2] = vec4(      look      , 0.0f);
	transform[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//convert back to modelspace
	bool ret = child->draw(model * transform);
	checkError("After Billboard Transform");
	return ret;
}
Drawable *BillboardTransform::copyStack() {
	BillboardTransform *copy = new BillboardTransform(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool ModelviewMode::draw(const mat4 &model) {
	int oldMode = Graphics::inst()->getModelviewMode();
	Graphics::inst()->setModelviewMode(mode);

	bool ret = child->draw(model);
	
	Graphics::inst()->setModelviewMode(oldMode);
	return ret;
}
Drawable *ModelviewMode::copyStack() {
	ModelviewMode *copy = new ModelviewMode(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool OffscreenObselescence::draw(const glm::mat4 &model) {
	vec4 eyePos = (Graphics::inst()->getView() * model) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return (eyePos.z <= 0 && child->draw(model));
}
Drawable *OffscreenObselescence::copyStack() {
	OffscreenObselescence *copy = new OffscreenObselescence(*this);
	copy->setChild(child->copyStack());
	return copy;
}

NoDeletion::~NoDeletion() {
	//Since this runs before ~DrawableDecorator,
	//setting the child to null prevents it from
	//being deleted, and therefore terminates the
	//deletion chain
	this->child = NULL;
}
void NoDeletion::takeDown() {}
Drawable *NoDeletion::copyStack() {
	NoDeletion *copy = new NoDeletion(*this);
	copy->setChild(child->copyStack());
	return copy;
}

bool ShaderUse::initialize() {
	shader = ShaderFlyweight::inst()->getShader(shaderName);
	return DrawableDecorator::initialize();
}
bool ShaderUse::draw(const mat4 &model) {
	//checkError("Before ShaderUse");
	Graphics::inst()->setShader(shader);
	bool ret = child->draw(model);
	glUseProgram(0);
	//checkError("After ShaderUse");
	return ret;
}
Drawable *ShaderUse::copyStack() {
	ShaderUse *copy = new ShaderUse(*this);
	copy->setChild(child->copyStack());
	return copy;
}
