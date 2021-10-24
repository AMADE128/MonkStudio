
#pragma once

#include "glmath.h"

struct Vertex
{
	vec3 m_pos;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, const vec3& normal)
	{
		m_pos = pos;
		m_normal = normal;
	}

	Vertex(const vec3& pos)
	{
		m_pos = pos;
		m_normal = vec3(0.0f, 0.0f, 0.0f);
	}
};