#ifndef obj_loader_h
#define obj_loader_h

#include "type_vec.h"
#include <vector>
#include <cstdio>
#include <cstring>
#include "GameObject.h"
#include <string>
#include <tchar.h>

using namespace std;

class ObjLoader
{
public:
	static GameObject* loadOBJ(wstring path);
private:
};

#endif