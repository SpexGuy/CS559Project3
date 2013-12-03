#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "ErrorCheck.h"
#include "Mesh.h"
#include "Shaders.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\mesh.h>
#include <assimp\postprocess.h>
using namespace std;
using namespace glm;

//#define DEBUG

Mesh *Mesh::newMars(float radius, float radScale, char *filename,
					Texture *texture, bool crosshatch) {

	ifstream inFile(filename);
	if (inFile.is_open()) {
		int width, height;
		inFile >> width;
		inFile >> height;
		cout << "file is " << width << 'x' << height << endl;
		vector<vector<float>> radii(height);
		for (int c = 0; c < height; c++) {
			radii[c] = vector<float>(width);
			for (int d = 0; d < width; d++) {
				if (!(inFile >> radii[c][d])) {
					cout << "Failed to read "<< c << ',' << d << endl;
					return NULL;
				}
			}
		}
		inFile.close();

		cout << "done reading" << endl;

		return Mesh::newMars(radius, radScale, radii, texture, crosshatch);
	} else {
		cerr << "Could not open file '" << filename << "'!" << endl;
		assert(false);
		return NULL;
	}
}

Mesh *Mesh::newMars(float radius, float radScale,
					const vector<vector<float>> &radii,
					Texture *texture,
					bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = radii.size();
	assert(height > 0);
	int width = radii[0].size();
	assert(width > 0);

	vector<vec3> points;
	vector<vec2> tex;

	for(int i = 0; i < height; i++) {
		for( int j = 0; j <= width; j++) {
			float r = radius + radScale*radii[i][j%width];
			float tScale = float(j) / float(width);
			float pScale = float(i+1) / float(height+1);
			float theta = float(2*M_PI * tScale);
			float phi = float(M_PI * pScale);

			float x = r*sin(theta)*sin(phi);
			float z = r*cos(theta)*sin(phi);
			float y = r*cos(phi);

			#ifdef DEBUG
				if(i < 1 && j < 10)
					cout << "( " << x << ", " << y << "," << z << " )" <<endl;
			#endif

			points.push_back(vec3(x,y,z));
			tex.push_back(vec2(tScale, pScale));
		}
	}

	#ifdef DEBUG
		cout << "Triangles" <<endl;
	#endif

	float topTotal = 0;
	float botTotal = 0;
	for (int c = 0; c < width; c++) {
		topTotal += radii[0][c];
		botTotal += radii.back()[c];
	}
	float topAvg = topTotal/float(width);
	float botAvg = botTotal/float(width);
	points.push_back(vec3(0, radius + topAvg*radScale, 0));
	tex.push_back(vec2(0, 0));
	points.push_back(vec3(0, -radius - botAvg*radScale, 0));
	tex.push_back(vec2(0, 1));

	vector<ivec3> trigs = generateTrigs(points, width+1, height, true, false, crosshatch);

	return new TexturedMesh(points, tex, trigs, texture);
}

Mesh *Mesh::newSphere(int stacks, int slices, float radius, Texture *tx, bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = stacks;
	assert(height > 0);
	int width = slices;
	assert(width > 0);

	
	vector<vec2> tex;
	vector<vec3> points;
	vector<vec3> norms;
	for(int i = 0; i< height; i++) {
		for( int j = 0; j <= width; j++) {
			float tScale = float(j) / float(width);
			float pScale = float(i+1) / float(height+1);
			float theta = float(2*M_PI * tScale);
			float phi = float(M_PI * pScale);

			float x = radius*sin(theta)*sin(phi);
			float z = radius*cos(theta)*sin(phi);
			float y = radius*cos(phi);

#ifdef DEBUG
			if(i < 1 && j < 10)
				cout << "( " << x << ", " << y << "," << z << " )" <<endl;
#endif
			tex.push_back(vec2(tScale, pScale));
			points.push_back(vec3(x,y,z));
			norms.push_back(normalize(vec3(x, y, z)));
		}
	}
#ifdef DEBUG
			
			cout << "Triangles" <<endl;
#endif
	
	points.push_back(vec3(0, radius, 0));
	tex.push_back(vec2(0.5, 0));
	norms.push_back(vec3(0,1,0));

	points.push_back(vec3(0, -radius, 0));
	tex.push_back(vec2(0.5, 1));
	norms.push_back(vec3(0,-1,0));

	vector<ivec3> trigs = generateTrigs(points, width+1, height, true, false, crosshatch);

	if (tx == NULL)
		return new Mesh(points, tex, trigs, norms);
	return new TexturedMesh(points, tex, trigs, norms, tx);
}

Mesh *Mesh::newCylinder(int stacks, int slices, float topRadius, float botRadius, bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = stacks;
	assert(height > 0);
	int width = slices;
	assert(width > 0);

	vector<vec2> tex;
	vector<vec3> points;
	float ratio = (topRadius-botRadius)/height;

	for(int i = 0; i <= height; i++) {
		for( int j = 0; j < width; j++) {
			float r = ((float)i*ratio) + (float)botRadius;
			float theta = float(2*M_PI * float(j) / float(width));

			float z = r*sin(theta);
			float x = r*cos(theta);
			float y = float(i)/float(height);

#ifdef DEBUG
			if(i < 1 && j < 10)
				cout << "( " << x << ", " << y << "," << z << " )" <<endl;
#endif
			points.push_back(vec3(x,y,z));
		}
	}
#ifdef DEBUG
			cout << "Triangles" <<endl;
#endif

	vector<ivec3> trigs = generateTrigs(points, width, height + 1, false, true, crosshatch);

	return new Mesh(points, tex, trigs);
}

Mesh *Mesh::newSurfaceOfRotation(const vector<vec2> &points,
								 int slices,
								 bool crosshatch) {
	vector<vec3> verts;
	vector<vec2> tex;
	for (uint c = 1; c < points.size()-1; c++) {
		for (int d = 0; d < slices; d++) {
			verts.push_back(
				vec3(points[c].y * cos(2*M_PI * float(d)/slices), 
					 points[c].x,
					 points[c].y * sin(2*M_PI * float(d)/slices)));
		}
	}
	verts.push_back(vec3(0.0f, points[0].x, 0.0f));
	verts.push_back(vec3(0.0f, points[points.size()-1].x, 0.0f));

	vector<ivec3> trigs = generateTrigs(verts, slices, points.size()-2, true, true, crosshatch);

	return new Mesh(verts, tex, trigs);
}

vector<ivec3> Mesh::generateTrigs(const vector<vec3> &points,
								  int width, int height,
								  bool endcaps, bool wrap,
								  bool crosshatch) {

	assert(int(points.size()) >= width*height + (endcaps ? 2 : 0));
	vector<ivec3> trigs;
	int endW = wrap ? width : width-1;
	for (int c = 0; c < height-1; c++) {
		for (int d = 0; d < endW; d++) {
			int tlIndex = c*width + d;
			int blIndex = tlIndex + width;
			int trIndex = c*width + (d+1)%width;
			int brIndex = trIndex + width;
			if (crosshatch && ((c%2 == 0) ^ (d%2 == 0))) {
				trigs.push_back(ivec3(tlIndex, blIndex, brIndex));
				trigs.push_back(ivec3(brIndex, trIndex, tlIndex));
			} else {
				trigs.push_back(ivec3(trIndex, tlIndex, blIndex));
				trigs.push_back(ivec3(blIndex, brIndex, trIndex));
			}

			#ifdef DEBUG
				if(c < 1 && (d > width - 1 || d < 1))
					cout << "( " << trIndex << ", " << tlIndex << "," << blIndex << " )" <<endl;
				if(c < 1 && (d > width - 1 || d < 1))
					cout << "( " << blIndex << ", " << brIndex << "," << trIndex << " )" <<endl<<endl;
			#endif
		}
	}

	if (endcaps) {
		int botRow = (height-1)*width;
		for (int c = 0; c < width; c++) {
			trigs.push_back(ivec3(c, (c+1)%width, points.size()-2));
			trigs.push_back(ivec3(points.size()-1, botRow + (c+1)%width, botRow + c));
		}
	}

	return trigs;
}


Mesh::Mesh(const vector<vec3> &ppoints,
		   const vector<vec2> &texCoords,
		   const vector<ivec3> &trigs) {

	assert(texCoords.size() == 0 || texCoords.size() == ppoints.size());
	this->drawNormals = false;
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);
	this->points = vector<VertexPNT>(ppoints.size());
	this->trigs = trigs;

	for (int c = 0; c < int(points.size()); c++) {
		points[c].position = ppoints[c];
		points[c].normal = vec3(0.0f);
		if (texCoords.size() > 0)
			points[c].texture = texCoords[c];
	}

	for (int c = 0; c < int(trigs.size()); c++) {
		vec3 vect1 = ppoints[trigs[c].y] - ppoints[trigs[c].z];
		vec3 vect2 = ppoints[trigs[c].y] - ppoints[trigs[c].x];
		vec3 planeNormal = cross(vect1, vect2);

		points[trigs[c].x].normal += planeNormal;
		points[trigs[c].y].normal += planeNormal;
		points[trigs[c].z].normal += planeNormal;
	}
	for (int c = 0; c < int(points.size()); c++) {
		if (points[c].normal == vec3(0.0f, 0.0f, 0.0f))
			points[c].normal = vec3(0.0f, 1.0f, 0.0f);
		points[c].normal = normalize(points[c].normal);
	}

	for (int c = 0; c < int(points.size()); c++) {
		normPoints.push_back(points[c].position);
		normPoints.push_back(points[c].position + vec3(0.1f)*points[c].normal);
		normSegs.push_back(ivec2(2*c, 2*c+1));
	}
}

Mesh::Mesh(const vector<vec3> &ppoints,
		   const vector<vec2> &texCoords,
		   const vector<ivec3> &trigs,
		   const vector<vec3> &norms) {

	assert(ppoints.size() == norms.size());
	assert(texCoords.size() == 0 || texCoords.size() == ppoints.size());
	points = vector<VertexPNT>(ppoints.size());
	for (int c = 0; c < (int)points.size(); c++) {
		points[c].normal = norms[c];
		points[c].texture = (texCoords.size() == 0 ? vec2(0,0) : texCoords[c]);
		points[c].position = ppoints[c];
	}

	this->trigs = trigs;
	
	this->drawNormals = false;
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);

}

Mesh::Mesh(const string filename)
{
	//Assimp::Importer importer;
	//const aiScene * ourMesh;
	//importer = Assimp::Importer::Importer();

	////read in the file!
	//ourMesh = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals); //might need to change the pFlags???
	//if(ourMesh == NULL)
	//	printf(importer.GetErrorString());
	////get the vertices!
	//for(int i = 0; i < (int)ourMesh->mNumMeshes; i++)
	//{
	//	for(int j = 0; j < (int)ourMesh->mMeshes[i]->mNumVertices; j++)
	//	{
	//		//grab the data to make points!
	//		aiVector3D tempV = ourMesh->mMeshes[i]->mVertices[j];
	//		aiVector3D tempN = ourMesh->mMeshes[i]->mNormals[j];
	//		aiVector3D tempT = ourMesh->mMeshes[i]->mTextureCoords[j][0]; //just grab the first TCs
	//		VertexPNT tempVPNT;
	//		tempVPNT.position = vec3(tempV.x, tempV.y, tempV.z);
	//		tempVPNT.normal = vec3(tempN.x, tempN.y, tempN.z);
	//		tempVPNT.texture = vec2(tempT.x, tempT.y);
	//		points.push_back(tempVPNT);
	//	}
	//	for(int j = 0; j < (int)ourMesh->mMeshes[i]->mNumFaces; j++)
	//	{
	//		aiFace tempFace = ourMesh->mMeshes[i]->mFaces[i];
	//		if(tempFace.mNumIndices != 3)
	//			printf("Not a triangle face! HOW DO I QUAD\n");
	//		ivec3 temptrig = ivec3(tempFace.mIndices[0], tempFace.mIndices[1], tempFace.mIndices[2]);
	//		trigs.push_back(temptrig);
	//	}
	//}
	

}

bool Mesh::initialize() {
	if (!Graphics::inst()->loadBuffer(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(VertexPNT), &this->points[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) (1*sizeof(vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) (2*sizeof(vec3)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normPoints.size() > 0) {
		if (!Graphics::inst()->loadBuffer(&this->normal_array_handle, &this->normal_coordinate_handle, this->normPoints.size() * sizeof(vec3), &this->normPoints[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	return true;
}

bool Mesh::draw(const mat4 &model) {
	if (checkError("Mesh::Draw - on entry"))
		return true;

	mat4 m = model;

	Graphics::inst()->drawTriangles(trigs, vertex_array_handle, m);

	if(drawNormals) {
		Graphics::inst()->drawLines(normSegs, normal_array_handle, m);
		if (checkError("Mesh::draw - on exit"))
			return true;
	}

	return true;
}

/*	Notice the destructor in this case asserts that all resources
	that don't go away by themselves have ALREADY been released. This
	is because the destructor might be called after a GL context has
	been destroyed, so I force the user of this class to call the
	TakeDown() purposefully.
*/
Mesh::~Mesh() {
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_coordinate_handle == GLuint(-1));
	assert(this->normal_array_handle == GLuint(-1));
	assert(this->normal_coordinate_handle == GLuint(-1));
}

void Mesh::takeDown() {
	if (this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);

	if (this->vertex_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_coordinate_handle);

	if (this->normal_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->normal_array_handle);

	if (this->normal_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_coordinate_handle);

	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);

}

bool TexturedMesh::initialize() {
	return Mesh::initialize();
}

bool TexturedMesh::draw(const mat4 &model) {
	if (checkError("TexturedMesh::Draw - on entry"))
		return true;

	mat4 m = model;

	texture->bindTexture();
	Graphics::inst()->drawTriangles(trigs, vertex_array_handle, m);

	if(drawNormals) {
		Graphics::inst()->drawLines(normSegs, normal_array_handle, m);
		if (checkError("TexturedMesh::draw - on exit"))
			return true;
	}

	return true;
}

