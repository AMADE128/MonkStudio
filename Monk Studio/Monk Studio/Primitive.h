
#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>
#include <cmath>

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
	~Sphere() {}

	void set(float radius, int sectorCount, int stackCount);
	void InnerRender() const;
private:
	void buildVerticesFlat();
	void buildInterleavedVertices();
	void clearArrays();
	void addVertex(float x, float y, float z);
	void addTexCoord(float s, float t);
	void addNormal(float x, float y, float z);
	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	std::vector<float> computeFaceNormal(float x1, float y1, float z1,
										 float x2, float y2, float z2,
										 float x3, float y3, float z3);
	// memeber vars
	float radius;
	int sectorCount;                        // longitude, # of slices
	int stackCount;                         // latitude, # of stacks
	bool smooth;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;

	// interleaved
	std::vector<float> interleavedVertices;
	int interleavedStride;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};