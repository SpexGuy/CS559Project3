/* See Graphics.h for function documentation */
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include "Graphics.h"
#include "ErrorCheck.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

using namespace glm;
using namespace std;

Graphics Graphics::instance;
Graphics *Graphics::inst() {
	return &instance;
}

Graphics::Graphics() {
	this->projection = mat4(1.0f);
	this->view = mat4(1.0f);
	this->light = vec3(0.0f);
	this->size = ivec2(1);
	this->color = BLACK;
	this->specularColor = WHITE;
	this->ambient = 1;
	this->diffuse = 0;
	this->shininess = 1;
	this->modelviewMode = MV_FULL;
	this->texIndex = 0;
	this->timeScale = 1;
	this->lastTrueTime = 0;
	this->time = 0;
	this->wireframe = false;

	circleCH = circleVH = 
	squareCH = squareVH =
	lineCH = lineVH = GLuint(-1);
}

bool Graphics::initialize() {

	//initialize the points for drawCircle2D, drawLine2D, and drawRect2D.
	vector<vec3> circlePoints(17);
	float angle;
	vector<vec3> linePoints;
	vector<vec3> squarePoints;

	circleTrigs = vector<ivec3>(16);
	lineSegs = vector<ivec2>(1);
	squareTrigs = vector<ivec3>(2);

	circlePoints[0] = vec3(0.0f);
	for (int c = 0; c < 16; c++) {
		angle = float(2 * M_PI * c / 16.0f);
		circlePoints[c+1] = vec3(cos(angle), sin(angle), 0.0f);
		circleTrigs[c] = ivec3(c+1, (c+1)%16 + 1, 0);
	}

	linePoints.push_back(vec3(0, 0, 0));
	linePoints.push_back(vec3(1, 1, 0));

	lineSegs.push_back(ivec2(0, 1));

	squarePoints.push_back(vec3(0, 0, 0));
	squarePoints.push_back(vec3(0, 1, 0));
	squarePoints.push_back(vec3(1, 1, 0));
	squarePoints.push_back(vec3(1, 0, 0));

	squareTrigs.push_back(ivec3(2, 1, 0));
	squareTrigs.push_back(ivec3(0, 3, 2));

	// load the circle onto the graphics card
	if (!loadBuffer(&circleVH, &circleCH, circlePoints.size() * sizeof(vec3), &circlePoints[0]))
		return false;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// load the square onto the graphics card
	if (!loadBuffer(&squareVH, &squareCH, squarePoints.size() * sizeof(vec3), &squarePoints[0]))
		return false;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// load the line onto the graphics card
	if (!loadBuffer(&lineVH, &lineCH, linePoints.size() * sizeof(vec3), &linePoints[0]))
		return false;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool Graphics::loadBuffer(GLuint *arrayHandle, GLuint *coordinateHandle, GLsizeiptr sz, const GLvoid *ptr) {
	glGenVertexArrays(1, arrayHandle);
	glBindVertexArray(*arrayHandle);
	glGenBuffers(1, coordinateHandle);
	glBindBuffer(GL_ARRAY_BUFFER, *coordinateHandle);
	glBufferData(GL_ARRAY_BUFFER, sz, ptr, GL_STATIC_DRAW);
	return !checkError("Graphics::loadBuffer - on exit");
}

void Graphics::drawWireCube() const {
	glutWireCube(2.0f);
}

void Graphics::setSize(const ivec2 &size) {
	mat4 vp = mat4(1.0f);
	vp = translate(vp, vec3(1.0f, 1.0f, 0.0f));
	vp = scale(vp, vec3(size, 1.0f));
	this->viewportmat = vp;
	this->size = size;
}

void Graphics::viewport(int x, int y, int width, int height) {
	this->setSize(ivec2(width, height));
	glViewport(x, y, width, height);
}

void Graphics::drawText2D(const mat4 &base, float x, float y, char *str, float size) const {
	////checkError("Before drawText");
	//mat4 pos = translate(base, vec3(x, y, 0));
	//float scaleFactor = size/72.0f;
	//pos = scale(pos, vec3(scaleFactor, scaleFactor, 1.0f));
	////iterate through the string
	//while(*str != '\0') {
	//	//checkError("before stroke character");
	//	currShader->setup(pos);
	//	//glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
	//	//translate past the character we just drew
	//	pos = translate(pos, vec3(glutStrokeWidth(GLUT_STROKE_ROMAN, *str), 0.0f, 0.0f));
	//	str++;
	//}
	//checkError("After drawText");
}

void Graphics::drawRect2D(const mat4 &base, vec2 blPoint, vec2 trPoint) const {
	vec2 diff = trPoint - blPoint;
	drawRect2D(base, blPoint.x, blPoint.y, diff.x, diff.y);
}

void Graphics::drawRect2D(const mat4 &base, float x, float y, float width, float height) const {
	//translate to the appropriate coordinates
	mat4 pos = translate(base, vec3(x, y, 0));
	//scale the square to be the correct rectangle
	pos = scale(pos, vec3(width, height, 1));
	//draw it
	drawTriangles(squareTrigs, squareVH, pos);
}

void Graphics::drawCircle2D(const mat4 &base, vec2 center, float radius) const {
	drawCircle2D(base, center.x, center.y, radius);
}

void Graphics::drawCircle2D(const mat4 &base, float x, float y, float radius) const {
	//translate to the center of the circle
	mat4 pos = translate(base, vec3(x, y, 0));
	//scale the circle to the correct size
	pos = scale(pos, vec3(radius, radius, 1));
	//draw it
	drawTriangles(circleTrigs, circleVH, pos);
}

void Graphics::drawLine2D(const mat4 &base, vec2 tlPoint, vec2 brPoint) const {
	//translate the line to the tlPoint
	mat4 pos = glm::translate(base, vec3(tlPoint, 0.0f));
	//the line is diagonal, so scale it to meet the bottom point
	pos = glm::scale(pos, vec3(brPoint - tlPoint, 1.0f));
	//draw it
	drawLines(lineSegs, lineVH, pos);
}

void Graphics::drawTriangles(const vector<ivec3> &trigs, const GLuint &vertexArrayHandle,
							 const mat4 &model) const {

	//checkError("Before drawTriangles");

	glBindVertexArray(vertexArrayHandle);

	currShader->setup(model);

	glDrawElements(
		GL_TRIANGLES,
		trigs.size()*3, //an ivec3 is three integers only, so pass an array of ivec3 as if it were an array of integers with 3 times the length
		GL_UNSIGNED_INT,
		&trigs[0]);

	glBindVertexArray(0);
	//checkError("After drawTriangles");

}

void Graphics::drawLines(const vector<ivec2> &segs, const GLuint &vertexArrayHandle,
						 const mat4 &model) const {


	glBindVertexArray(vertexArrayHandle);

	currShader->setup(model);

	glDrawElements(
		GL_LINES,
		segs.size()*2,
		GL_UNSIGNED_INT,
		&segs[0]);

	glBindVertexArray(0);

	//checkError("Graphics::draw - after drawLines");
}

void Graphics::drawPoints(const vector<int> &points, const GLuint &vertexArrayHandle,
						 const mat4 &model) const {


	glBindVertexArray(vertexArrayHandle);

	currShader->setup(model);

	glDrawElements(
		GL_POINTS,
		points.size(),
		GL_UNSIGNED_INT,
		&points[0]);

	glBindVertexArray(0);

	//checkError("Graphics::draw - after draw points");
}


vec3 Graphics::getLightPos() {
	return vec3(view * vec4(light,1.0f));
}

mat4 Graphics::getModelview(const mat4 &model) {
	mat4 modelview = view * model;

	if (modelviewMode == MV_ROTATION) {
		//remove the translations from the modelview
		modelview[3][0] = 0.0f;
		modelview[3][1] = 0.0f;
		modelview[3][2] = 0.0f;
	}

	return modelview;
}

mat3 Graphics::getNormalMatrix(const mat4 &mv) {
	return inverse(transpose(mat3(mv)));
}


void Graphics::takeDown() {
	//delete the 2D geometry buffers
	glDeleteVertexArrays(1, &circleVH);
	glDeleteBuffers(1, &circleCH);
	glDeleteVertexArrays(1, &squareVH);
	glDeleteBuffers(1, &squareCH);
	glDeleteVertexArrays(1, &lineVH);
	glDeleteBuffers(1, &lineCH);

	circleCH = circleVH = 
	squareCH = squareVH =
	lineCH = lineVH = GLuint(-1);
}

Graphics::~Graphics() {
	assert(circleCH == GLuint(-1) &&
			circleVH == GLuint(-1) &&
			squareCH == GLuint(-1) &&
			squareVH == GLuint(-1) &&
			lineCH == GLuint(-1) &&
			lineVH == GLuint(-1));
}
