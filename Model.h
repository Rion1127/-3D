#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <list>
using namespace DirectX;
class Obj
{
public:
	static Obj* GetInstance();

	bool loadOBJ(const std::string& modelname);

	struct Vertex {
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//uv座標
	};
	std::vector<Vertex> obj;

	
private:
	std::vector<uint16_t> vertexIndices, uvIndices, normalIndices;
	std::vector<XMFLOAT3> temp_vertices;
	std::vector<XMFLOAT3> temp_normals;
	std::vector<XMFLOAT2> temp_uvs;

	

};



