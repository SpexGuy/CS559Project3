#include "Ribbon.h"
#include "Drawable.h"
#include "Spline.h"
#include "Mesh.h"
#include <vector>

using namespace std;
using namespace glm;

void RibbonBuilder::update() {
	int time = Graphics::inst()->getTime();
	while(time - lastSpawnTime > segmentTime) {
		lastSpawnTime += segmentTime;
		spawn(lastSpawnTime);
	}
}

void RibbonBuilder::spawn(int time) {
	assert(time % segmentTime == 0);
	//generate new points
	SplinePoint3 newPoint = generatePoint();
	SplinePoint1 newAngle = generateAngle();
	//generate mesh
	Drawable *mesh = makeRibbonSegment(newPoint, newAngle)
		->translated(vec3(0.0f, 0.0f, -segmentLength * time/segmentTime))
		->obselesceOffscreen()
		->drawZOrdered()
		->setGlBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		->useShader(SHADER_NOISE);


	//translate mesh into position
	ribbons->addElement(mesh);
	//trigger obselescence
	//add to group
	mesh->initialize();
}

bool RibbonBuilder::initialize() {
	return ribbonStack->initialize();
}








RibbonBuilder::RibbonBuilder(
				DrawableGroup *container,
				glm::vec3 position,
				float velocity,
				float length,
				float width,
				int resolution,
				vec3 arbitrary,
				vec3 startAbsolute,
				Texture *texture) :

		lastSpawnTime(0),
		segmentLength(length),
		segmentTime(length/velocity),
		width(width),
		resolution(resolution),
		arb(normalize(arbitrary)),
		absolute(normalize(startAbsolute)),
		texture(texture)
{
	lastRibbonPoint = generatePoint();
	lastRibbonAngle = generateAngle();
	lastPoint = lastRibbonPoint.getBeforePoint();
	assert(arb != absolute);

	ribbons = new DrawableGroup();
		container->addElement(
			ribbonStack = 
				ribbons
					->animateTranslation(
						new Vec3TimeFunction(
							new ConstantTimeFunction(position.x),
							new ConstantTimeFunction(position.y),
							new LinearTimeFunction(velocity, position.z)))
					->inMaterial(0.2f, 0.5f, 50.0f)
	);
}

Mesh *RibbonBuilder::makeRibbonSegment(
				const SplinePoint3 &after,
				const SplinePoint1 &afterAngle)
{
	lastRibbonPoint.position.z += segmentLength;
	lastPoint.z += segmentLength;
	vector<vec3> points;
	vector<vec2> texCoords;
	for (int c = 0; c <= resolution; c++) {
		float t = float(c)/resolution;
		vec3 point = bezier(lastRibbonPoint, after, t);
		float angle = bezier(lastRibbonAngle, afterAngle, t);
		vec3 tangent = normalize(lastPoint - point);
		if (tangent != arb)
			absolute = normalize(cross(tangent, arb));
		vec3 rotabs = vec3(rotate(mat4(1.0f), angle, tangent) * vec4(absolute, 1.0f));

		points.push_back(point + rotabs * (width/2));
		texCoords.push_back(vec2(t, 0));
		points.push_back(point - rotabs * (width/2));
		texCoords.push_back(vec2(t, 1));

		if (c != resolution)
			lastPoint = point;
	}

	vector<ivec3> trigs = Mesh::generateTrigs(points, 2, resolution+1, false, false, false);

	lastRibbonPoint = after;
	lastRibbonAngle = afterAngle;

	if (texture == NULL)
		return new Mesh(points, texCoords, trigs);
	else
		return new TexturedMesh(points, texCoords, trigs, texture);
}

SplinePoint3 RibbonBuilder::generatePoint() {
	SplinePoint3 point;
	point.angle = 178 * (float(rand())/RAND_MAX) + 91;
	point.axisAngle = 180 * (float(rand())/RAND_MAX);
	point.distance = 1 + 2*(float(rand())/RAND_MAX);
	point.position = vec3(0.0f);
	return point;
}

SplinePoint1 RibbonBuilder::generateAngle() {
	SplinePoint1 point;
	point.position = 360 * (float(rand())/RAND_MAX) - 180;
	point.distance = 0.5f + float(rand())/RAND_MAX;
	return point;
}