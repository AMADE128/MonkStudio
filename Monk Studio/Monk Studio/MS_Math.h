
#pragma once

#include "glmath.h"

struct Vertex
{
	vec3 m_pos;
	vec3 m_normal;
	vec2 m_tex;

	Vertex() {}

	Vertex(const vec3& pos, const vec3& normal, const vec2& tex)
	{
		m_pos = pos;
		m_normal = normal;
		m_tex = tex;
	}

	Vertex(const vec3& pos, const vec2& tex)
	{
		m_pos = pos;
		m_normal = vec3(0.0f, 0.0f, 0.0f);
		m_tex = tex;
	}
};