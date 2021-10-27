
#pragma once

#include "glmath.h"

struct Vertex
{
	vec3 m_pos;
	vec2 m_tex;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, const vec2& tex, const vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}

	Vertex(const vec3& pos, const vec2& tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = vec3(0.0f, 0.0f, 0.0f);
	}
};