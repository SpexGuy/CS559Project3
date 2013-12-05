#pragma once
#include <vector>
#include "Shaders.h"
#include "Vertex.h"
#include "Drawable.h"
#include "Texture.h"
#include "Spline.h"
#include <glm/glm.hpp>

class Mesh : public Drawable {
private:
	Mesh();
protected:

	bool drawNormals;
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	GLuint normal_coordinate_handle;
	GLuint normal_array_handle;

	std::vector<VertexPNT> points;
	std::vector<glm::ivec3> trigs;
	std::vector<glm::vec3> normPoints;
	std::vector<glm::ivec2> normSegs;
	

public:
	/** generates a mesh with the given points. Trigs is a vector of
	 * triangles.  A triangle is a vec3 of indexes in <points>
	 * calculates triangle normals by taking the vector from trig[1]
	 * to trig[2] and crossing it with the vector from trig[1] to trig[0].
	 * Calculates point normals by averaging the vectors from all triangles
	 * which have this point as a vertex. */
	Mesh(const std::vector<glm::vec3> &points,
		 const std::vector<glm::vec2> &texCoords,
		 const std::vector<glm::ivec3> &trigs);

	Mesh(const std::vector<glm::vec3> &points,
		 const std::vector<glm::vec2> &texCoords,
		 const std::vector<glm::ivec3> &trigs,
		 const std::vector<glm::vec3> &norms);



	virtual bool initialize();
	
	virtual bool draw(const glm::mat4 &modelview);

	virtual void takeDown();

	inline void setDrawNormals(bool dn) {
		this->drawNormals = dn;
	}
	inline bool isDrawingNormals() {
		return drawNormals;
	}

	virtual ~Mesh();



	//---------Static Members------------------------

	/** creates a spherical Mesh with a base radius offset by data
	 * from the given file, scaled by radScale.
	 *
	 * The file format is as follows:
	 * The first line is the width followed by the height of the data,
	 * separated by a tab. There should be exactly <height> more lines,
	 * each consisting of <width> float values between 0 and 1, separated
	 * by tabs.
	 * Returns */
	static Mesh *newMars(float radius, float radScale,
		char *filename, Texture *texture,
		bool crosshatch = false);

	/** creates a spherical mesh with the radius at point i,j
	 * calculated as radius + radScale*radii[i][j]. 
	 */
	static Mesh *newMars(float radius, float radScale,
		const std::vector<std::vector<float>> &radii,
		Texture *texture,
		bool crosshatch = false);

	/** creates a cylinder mesh with a certain number of stacks and slices,
	 *  a top radius and bottom radius. The origin of the cylinder is the 
	 *  center of the top slice. 
	 */
	static Mesh *newCylinder(int stacks, int slices, 
				float topRadius, float botRadius,
				bool crosshatch = false);

	/** creates a sphere mesh with a certain number of stacks and slices,
	 * along with a radius. The origin of the sphere is center of the sphere.
	 */
	static Mesh *newSphere(int stacks, int slices,
		float radius, Texture *tx = NULL,
		bool crosshatch = false);

	/** creates a surface of rotation mesh with a certain number of slices,
	 * the stacks are that are is based off the points passed in.  
	 */
	static Mesh *newSurfaceOfRotation(
		const std::vector<glm::vec2> &points,
		int slices, bool crosshatch = false);

	/** creates a vector of pairs of indices that correspond to the vector of points to describe triangles. 
	 */
	static std::vector<glm::ivec3> generateTrigs(
		const std::vector<glm::vec3> &points,
		int width, int height,
		bool endcaps, bool wrap,
		bool crosshatch = false);

	/* creates a mesh object from an external .obj file */
	/* Texture can be null - kinda like the ribbon constructor I think? */
	static Mesh *newMeshFromFile(const string filename, Texture * texture, bool reCenterMesh = false, bool genTang = false);

	/* These are some utility functions taken from the cookbook - used by the obj importing mesh constructor. */
	/* could also be used by general mesh constructor if we need the mesh to store tangents???*/
	//Not sure if these should be static?
	//does something to a string???
	static void trimString(string & string);
	//recenters a mesh
	static void center(std::vector<glm::vec3> & points);
	//generates averaged normals!
	static void generateAveragedNormals(
        const std::vector<glm::vec3> & points,
        std::vector<glm::vec3> & normals,
        const std::vector<glm::ivec3> & faces );
	//generates tangents
	static void generateTangents(
        const std::vector<glm::vec3> & points,
        const std::vector<glm::vec3> & normals,
        const std::vector<glm::ivec3> & faces,
        const std::vector<glm::vec2> & texCoords,
        std::vector<glm::vec4> & tangents);
};

class TexturedMesh : public Mesh {
private:
	TexturedMesh();
protected:
	Texture *texture;
public:
	TexturedMesh(const std::vector<glm::vec3> &points,
				 const std::vector<glm::vec2> &texCoords,
				 const std::vector<glm::ivec3> &trigs,
				 Texture *texture) :
		Mesh(points, texCoords, trigs),
		texture(texture)
	{}

	TexturedMesh(const std::vector<glm::vec3> &points,
				 const std::vector<glm::vec2> &texCoords,
				 const std::vector<glm::ivec3> &trigs,
				 const std::vector<glm::vec3> &norms,
				 Texture *texture) :
		Mesh(points, texCoords, trigs, norms),
		texture(texture)
	{}

	virtual bool initialize();
	
	virtual bool draw(const glm::mat4 &model);

};



void primeRibbonBuilder(SplinePoint3 firstPoint,
						SplinePoint1 firstAngle,
						glm::vec3 arbitrary,
						glm::vec3 startAbsolute);

Mesh *makeRibbonSegment(const SplinePoint3 &after,
				 const SplinePoint1 &afterAngle,
				 float width,
				 int resolution,
				 Texture *texture = NULL);

