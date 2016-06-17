#include "ObjLoader.h"

GameObject* ObjLoader::loadOBJ(const wstring path)
{
	std::vector<unsigned int> vertexIndices,uvIndices;	//顶点和纹理通过索引知道其值，因为模型文件为了共用顶点而使用索引

	std::vector<vec3> temp_vertices;	//存储所有顶点坐标
	std::vector<vec2> temp_uvs;			//存储所有顶点对应的纹理坐标

	std::vector<vec3> out_vertices;		//存储所有三角形对应顶点坐标，个数：3*三角形个数
	std::vector<vec2> out_uvs;			//存储所有三角形的顶点对应的纹理坐标

	FILE* file = _wfopen(path.c_str(),_T("r"));
	if (file == NULL)
	{
		printf("obj loaded unsuccessfully");
		getchar();
		return nullptr;
	}

	while (1)
	{
		char lineHeader[128];

		int res = fscanf(file,"%s",lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader,"v") == 0)	//顶点坐标			v 1.000000 -1.000000 -1.000000
		{
			vec3 vertex;
			fscanf(file,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader,"vt") == 0)	//顶点纹理坐标		vt 0.750412 0.748571
		{
			vec2 uv;
			fscanf(file,"%f %f\n",&uv.u,&uv.v);

			uv.v = -uv.v;	//DDS 是反的. TGA和BMP是正的 

			temp_uvs.push_back(uv);

		}
		else if (strcmp(lineHeader,"vn") == 0)	//顶点法线				vn 0.000000 1.000000 -0.000000
		{
			//暂时不处理
		}
		else if (strcmp(lineHeader,"f") == 0)	//三角形索引			f 5/1/1 1/2/1 4/3/1
		{
			unsigned int vertexIndex[3],uvIndex[3],normalIndex[3];		//这个三角形的索引
			int matches = fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&vertexIndex[0],&uvIndex[0],&normalIndex[0],
				&vertexIndex[1],&uvIndex[1],&normalIndex[1],&vertexIndex[2],&uvIndex[2],&normalIndex[2]);
			if (matches !=9)
			{
				printf("file can't be read by simple parser");
				return nullptr;
			}
			vertexIndices.push_back(vertexIndex[0]);		//插入三个三角形的索引
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
		}
		else			//空行
		{
			char stupidBuffer[1000];
			fgets(stupidBuffer,1000,file);
		}
	}

	//通过索引确定具体的顶点数据
	for (unsigned int i = 0; i<vertexIndices.size(); i++)	//3*三角形个数
	{
		//获取索引值
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];

		//获取具体数据
		vec3 vertex = temp_vertices[vertexIndex-1];		//OBJ文件索引从1开始
		vec2 uv = temp_uvs[uvIndex-1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
	}


	//并设置GameObject
	GameObject* go = new GameObject(out_vertices,out_uvs);
	return go;
}