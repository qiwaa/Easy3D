#include "GameObject.h"
#include <cstring>

GameObject::GameObject(const std::vector<vec3>& out_vertexs,const std::vector<vec2>& out_uvs)
{
	vertexs.resize(out_vertexs.size());
	memcpy(&vertexs[0], &out_vertexs[0], out_vertexs.size() * sizeof(vec3));

	uvs.resize(out_uvs.size());
	memcpy(&uvs[0], &out_uvs[0], out_uvs.size() * sizeof(vec2));
}

const std::vector<vec3> GameObject::GetVertexs()
{
	return vertexs;
}
const std::vector<vec2> GameObject::GetUvs()
{
	return uvs;
}