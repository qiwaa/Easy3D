#ifndef _game_object_h
#define _game_object_h

#include "type_vec.h"
#include <vector>

class GameObject
{
public:
	GameObject(const std::vector<vec3>& out_vertexs,const std::vector<vec2>& out_uvs);
	const std::vector<vec3> GetVertexs();
	const std::vector<vec2> GetUvs();
private:
	std::vector<vec3> vertexs;
	std::vector<vec2> uvs;
};

#endif