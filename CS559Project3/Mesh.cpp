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


SplinePoint3 lastRibbonPoint;
SplinePoint1 lastRibbonAngle;
vec3 arb;
vec3 absolute;
vec3 lastPoint;

void primeRibbonBuilder(SplinePoint3 firstPoint,
						SplinePoint1 firstAngle,
						vec3 arbitrary,
						vec3 startAbsolute)
{
	lastRibbonPoint = firstPoint;
	lastRibbonAngle = firstAngle;
	arb = normalize(arbitrary);
	absolute = normalize(startAbsolute);
	lastPoint = firstPoint.getBeforePoint();
	assert(arb != absolute);
}

Mesh *makeRibbonSegment(
				const SplinePoint3 &after,
				const SplinePoint1 &afterAngle,
				float width,
				int resolution,
				Texture *texture)
{
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
//Taken from OpenGL Cookbook!
Mesh * Mesh::newMeshFromFile(const string filename, Texture * texture, bool reCenterMesh, bool genTang)
{
	vector <vec3> points;
    vector <vec3> normals;
    vector <vec2> texCoords;
    vector <ivec3> faces;

    int nFaces = 0;

    ifstream objStream( filename, std::ios::in );

    if( !objStream ) {
        cerr << "Unable to open OBJ file: " << filename << endl;
        exit(1);
    }

    string line, token;
    vector<int> face;

    getline( objStream, line );
    while( !objStream.eof() ) {
        trimString(line);
        if( line.length( ) > 0 && line.at(0) != '#' ) {
            istringstream lineStream( line );

            lineStream >> token;

            if (token == "v" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                points.push_back( vec3(x,y,z) );
            } else if (token == "vt" && texture != NULL) {
                // Process texture coordinate
                float s,t;
                lineStream >> s >> t;
                texCoords.push_back( vec2(s,t) );
            } else if (token == "vn" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.push_back( vec3(x,y,z) );
            } else if (token == "f" ) {
                nFaces++;

                // Process face
                face.clear();
                size_t slash1, slash2;
                //int point, texCoord, normal;
                while( lineStream.good() ) {
                    string vertString;
                    lineStream >> vertString;
                    int pIndex = -1, nIndex = -1 , tcIndex = -1;

                    slash1 = vertString.find("/");
                    if( slash1 == string::npos ){
                        pIndex = atoi( vertString.c_str() ) - 1;
                    } else {
                        slash2 = vertString.find("/", slash1 + 1 );
                        pIndex = atoi( vertString.substr(0,slash1).c_str() )
                                        - 1;
                        if( slash2 > slash1 + 1 ) {
                                tcIndex =
                                        atoi( vertString.substr(slash1 + 1, slash2).c_str() )
                                        - 1;
                        }
                        nIndex =
                                atoi( vertString.substr(slash2 + 1,vertString.length()).c_str() )
                                - 1;
                    }
                    if( pIndex == -1 ) {
                        printf("Missing point index!!!");
                    } else {
                        face.push_back(pIndex);
                    }

                    if( texture != NULL && tcIndex != -1 && pIndex != tcIndex ) {
                        printf("Texture and point indices are not consistent.\n");
                    }
                    if ( nIndex != -1 && nIndex != pIndex ) {
                        printf("Normal and point indices are not consistent.\n");
                    }
                }
                // If number of edges in face is greater than 3,
                // decompose into triangles as a triangle fan.
                if( face.size() > 3 ) {
                    int v0 = face[0];
                    int v1 = face[1];
                    int v2 = face[2];
                    // First face
                    faces.push_back(ivec3(v0,v1,v2));
                    for( GLuint i = 3; i < face.size(); i++ ) {
                        v1 = v2;
                        v2 = face[i];
						faces.push_back(ivec3(v0,v1,v2));
                    }
                } else {
                    faces.push_back(ivec3(face[0], face[1], face[2]));
                }
            }
        }
        getline( objStream, line );
    }

    objStream.close();

    if( normals.size() == 0 || points.size() != normals.size()) {
		normals.clear();
        generateAveragedNormals(points,normals,faces);
    }

    vector<vec4> tangents;
    if( genTang && texCoords.size() > 0 ) {
        generateTangents(points,normals,faces,texCoords,tangents);
    }

    if( reCenterMesh ) {
        center(points);
    }

    cout << "Loaded mesh from: " << filename << endl;
    cout << " " << points.size() << " points" << endl;
    cout << " " << nFaces << " faces" << endl;
    cout << " " << faces.size() / 3 << " triangles." << endl;
    cout << " " << normals.size() << " normals" << endl;
    cout << " " << tangents.size() << " tangents " << endl;
    cout << " " << texCoords.size() << " texture coordinates." << endl;

	if(texture != NULL)
		return new TexturedMesh(points, texCoords, faces, normals, texture);
	else
		return new Mesh(points, texCoords, faces, normals);
}

void Mesh::trimString( string & str ) {
    const char * whiteSpace = " \t\n\r";
    size_t location;
    location = str.find_first_not_of(whiteSpace);
    str.erase(0,location);
    location = str.find_last_not_of(whiteSpace);
    str.erase(location + 1);
}

void Mesh::generateTangents(
        const vector<vec3> & points,
        const vector<vec3> & normals,
        const vector<ivec3> & faces,
        const vector<vec2> & texCoords,
        vector<vec4> & tangents)
{
    vector<vec3> tan1Accum;
    vector<vec3> tan2Accum;

    for( uint i = 0; i < points.size(); i++ ) {
        tan1Accum.push_back(vec3(0.0f));
        tan2Accum.push_back(vec3(0.0f));
        tangents.push_back(vec4(0.0f));
    }

    // Compute the tangent vector
    for( uint i = 0; i < faces.size(); i++ )
    {
        const vec3 &p1 = points[faces[i].x];
        const vec3 &p2 = points[faces[i].y];
        const vec3 &p3 = points[faces[i].z];

		const vec2 &tc1 = texCoords[faces[i].x];
		const vec2 &tc2 = texCoords[faces[i].y];
        const vec2 &tc3 = texCoords[faces[i].z];

        vec3 q1 = p2 - p1;
        vec3 q2 = p3 - p1;
        float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
        float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
        float r = 1.0f / (s1 * t2 - s2 * t1);
        vec3 tan1( (t2*q1.x - t1*q2.x) * r,
                   (t2*q1.y - t1*q2.y) * r,
                   (t2*q1.z - t1*q2.z) * r);
        vec3 tan2( (s1*q2.x - s2*q1.x) * r,
                   (s1*q2.y - s2*q1.y) * r,
                   (s1*q2.z - s2*q1.z) * r);
        tan1Accum[faces[i].x] += tan1;
        tan1Accum[faces[i].y] += tan1;
        tan1Accum[faces[i].z] += tan1;
        tan2Accum[faces[i].x] += tan2;
        tan2Accum[faces[i].y] += tan2;
        tan2Accum[faces[i].z] += tan2;
    }

    for( uint i = 0; i < points.size(); ++i )
    {
        const vec3 &n = normals[i];
        vec3 &t1 = tan1Accum[i];
        vec3 &t2 = tan2Accum[i];

        // Gram-Schmidt orthogonalize
        tangents[i] = vec4(glm::normalize( t1 - (glm::dot(n,t1) * n) ), 0.0f);
        // Store handedness in w
        tangents[i].w = (glm::dot( glm::cross(n,t1), t2 ) < 0.0f) ? -1.0f : 1.0f;
    }
    tan1Accum.clear();
    tan2Accum.clear();
}

void Mesh::center( vector<vec3> & points ) {
    if( points.size() < 1) return;

    vec3 maxPoint = points[0];
    vec3 minPoint = points[0];

    // Find the AABB
    for( uint i = 0; i < points.size(); ++i ) {
        vec3 & point = points[i];
        if( point.x > maxPoint.x ) maxPoint.x = point.x;
        if( point.y > maxPoint.y ) maxPoint.y = point.y;
        if( point.z > maxPoint.z ) maxPoint.z = point.z;
        if( point.x < minPoint.x ) minPoint.x = point.x;
        if( point.y < minPoint.y ) minPoint.y = point.y;
        if( point.z < minPoint.z ) minPoint.z = point.z;
    }

    // Center of the AABB
    vec3 center = vec3( (maxPoint.x + minPoint.x) / 2.0f,
                        (maxPoint.y + minPoint.y) / 2.0f,
                        (maxPoint.z + minPoint.z) / 2.0f );

    // Translate center of the AABB to the origin
    for( uint i = 0; i < points.size(); ++i ) {
        vec3 & point = points[i];
        point = point - center;
    }
}

void Mesh::generateAveragedNormals(
        const vector<vec3> & points,
        vector<vec3> & normals,
        const vector<ivec3> & faces )
{
    for( uint i = 0; i < points.size(); i++ ) {
        normals.push_back(vec3(0.0f));
    }

    for( uint i = 0; i < faces.size(); i++) {
        const vec3 & p1 = points[faces[i].x];
        const vec3 & p2 = points[faces[i].y];
        const vec3 & p3 = points[faces[i].z];

        vec3 a = p2 - p1;
        vec3 b = p3 - p1;
        vec3 n = glm::normalize(glm::cross(a,b));

		normals[faces[i].x] += n;
		normals[faces[i].y] += n;
		normals[faces[i].z] += n;
    }

    for( uint i = 0; i < normals.size(); i++ ) {
        normals[i] = glm::normalize(normals[i]);
    }
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

