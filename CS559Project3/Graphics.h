#pragma once
#include <vector>
#include <map>
#include "Shaders.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

// Some common colors
#define RED    vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define YELLOW vec4(1.0f, 1.0f, 0.0f, 1.0f)
#define GREEN  vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE   vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define BLACK  vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define WHITE  vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define MARS   vec4(1.0f, 0.2f, 0.2f, 1.0f)

// modelview modes
#define MV_FULL 7
#define MV_ROTATION 2

// number of lights!
#define NUM_LIGHTS 5

class Drawable;
/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 * It also serves to accumulate state for the shaders.
 */
class Graphics {

public:

	/* Struct for z-ordered drawables */
	typedef struct{
		Drawable * object;
		glm::mat4 mvmatrix;
	} ZDrawable;


private:

	GLSLProgram *currShader;

	glm::mat4 projection;
	glm::mat4 view;

	glm::mat4 viewportmat;
	
	glm::ivec2 size;

	glm::vec4 color;
	
	glm::vec3 light[NUM_LIGHTS];
	glm::vec4 lightColor[NUM_LIGHTS];
	float ambient, diffuse, specular, shininess;

	int modelviewMode;

	int texIndex;

	float timeScale;
	int lastTrueTime;
	int time;

	bool wireframe;
	bool flipUCoords;

	bool paused;

	std::vector<glm::ivec3> circleTrigs;
	std::vector<glm::ivec3> squareTrigs;
	std::vector<glm::ivec2> lineSegs;
	GLuint squareVH;
	GLuint squareCH;
	GLuint circleVH;
	GLuint circleCH;
	GLuint lineVH;
	GLuint lineCH;

	static Graphics instance;

	std::multimap<float, ZDrawable> zOrderDrawables; 

	/* creates a Graphics instance but DOES NOT INITIALIZE IT */
	Graphics();


public:

	/* Graphics is a Singleton */
	static Graphics *inst();

	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	/* loads the given buffer onto the graphics card
	 * will overwrite vertexHandle and coordinateHandle
	 * with generated values. */
	bool loadBuffer(GLuint *vertexHandle, GLuint *coordinateHandle,
		GLsizeiptr size, const GLvoid *data);

	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1),
	 * using legacy GL */
	void drawWireCube() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels). */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	/* draws a rectangle with the given corners */
	void drawRect2D(const glm::mat4 &base, glm::vec2 blPoint, glm::vec2 trPoint) const;
	/* draws a rectangle at the given position with the given size */
	void drawRect2D(const glm::mat4 &base, float x, float y, float width, float height) const;

	/* draws a circle with the given center and radius */
	void drawCircle2D(const glm::mat4 &base, glm::vec2 center, float radius) const;
	/* draws a circle at the given coordinates with the given radius */
	void drawCircle2D(const glm::mat4 &base, float x, float y, float radius) const;

	/* draws a line between the two points */
	void drawLine2D(const glm::mat4 &base, glm::vec2 tlPoint, glm::vec2 brPoint) const;

	/* draws triangles with the given array and shader
	 * using the given model space */
	void drawTriangles(const std::vector<glm::ivec3> &trigs,
					   const GLuint &vertexArrayHandle,
					   const glm::mat4 &model)
				const;

	/* draws lines with the given array and shader
	 * using the given model space */
	void drawLines(const std::vector<glm::ivec2> &segs,
				   const GLuint &vertexArrayHandle,
				   const glm::mat4 &model)
				const;

	/* draws points with the given array and shader
	 * using the given model space */
	void drawPoints(const std::vector<int> &points,
					const GLuint &vertexArrayHandle,
					const glm::mat4 &model)
				const;

	/* generates a glViewport with the given parameters.
	 * also sets the size to (width, heigth). */
	void viewport(int x, int y, int width, int height);

	/* frees resources */
	void takeDown();

	/* asserts that takeDown() has been called */
	~Graphics();

	/* Inserts a ZDrawable into the ZOrderDrawables multimap */
	void addZDrawable(Drawable * child, mat4 model);

	/* Clears the zOrderDrawables multimap */
	void clearZDrawables();

	/* Draws all of the zDrawables */
	bool drawZDrawables();
	
	//---------------- getters and setters -----------------

	inline void setProjection(const glm::mat4 &projection) {
		this->projection = projection;
	}
	inline void setView(const glm::mat4 &view) {
		this->view = view;
	}
	inline void setColor(const glm::vec4 &color) {
		this->color = color;
	}
	//For the setLight/setMaterial stuff, now we need to specify an index into the array! i'll do that later.
	inline void setLight(int index, const glm::vec3 &light, const glm::vec4 &s) {
		this->light[index] = vec3(view * vec4(light, 1.0f));
		this->lightColor[index] = s;
	}
	inline void setMaterial(const float &a, const float &s, const float &shiny) {
		assert(a <= 1);
		this->ambient = a;
		this->diffuse = 1-a;
		this->specular = s;
		this->shininess = shiny;
	}
	inline void setModelviewMode(int mode) {
		modelviewMode = mode;
	}
	inline void update() {
		int newTime = glutGet(GLUT_ELAPSED_TIME);
		if (!paused)
			time += int(timeScale * (newTime - lastTrueTime));
		lastTrueTime = newTime;
	}
	inline void togglePause() {
		paused = !paused;
	}
	inline void resetTime() {
		this->time = 0;
	}
	inline void setTimeScale(float timeScale) {
		this->timeScale = timeScale;
	}
	inline void setTexture(int texIndex) {
		this->texIndex = texIndex;
	}
	inline void setFlipU(bool flipU) {
		flipUCoords = flipU;
	}
	void setSize(const glm::ivec2 &size);
	void clearLights();



	inline bool isFlipU() const {
		return flipUCoords;
	}
	inline glm::mat4 getProjection() const {
		return projection;
	}
	inline glm::mat4 getView() const {
		return view;
	}
	inline glm::ivec2 getSize() const {
		return size;
	}
	inline glm::vec4 getColor() const {
		return color;
	}
	inline glm::vec3 getLight(int index) const {
		return light[index];
	}
	inline float getAmbient() const {
		return ambient;
	}
	inline float getDiffuse() const {
		return 1-ambient;
	}
	inline float getSpecular() const {
		return specular;
	}
	inline float getShininess() const {
		return shininess;
	}
	inline glm::vec4 getLightColor(int index) const {
		return lightColor[index];
	}
	inline int getModelviewMode() const {
		return modelviewMode;
	}
	inline int getTime() const {
		return time;
	}
	inline float getTimeScale() {
		return timeScale;
	}
	inline int getTexture() const {
		return texIndex;
	}
	inline glm::vec3 getAmbientVec() {
		return glm::vec3(getAmbient());
	}
	inline glm::vec3 getDiffuseVec() {
		return glm::vec3(getDiffuse());
	}
	inline glm::vec3 getSpecularVec() {
		return glm::vec3(getSpecular());
	}
	inline bool isWireframe() {
		return wireframe;
	}
	inline void setWireframe(bool wf) {
		wireframe = wf;
	}
	inline glm::mat4 getViewportMatrix() {
		return viewportmat;
	}
	inline void setShader(GLSLProgram *shader) {
		currShader = shader;
		currShader->use();
	}
	
	inline glm::vec4 * getLightColorArray() {
		return lightColor;
	}
	inline glm::vec3 * getLightArray() {
		return light;
	}



	glm::vec3 getLightPos(int index);
	glm::mat4 getModelview(const glm::mat4 &model);
	glm::mat3 getNormalMatrix(const glm::mat4 &mv);

};