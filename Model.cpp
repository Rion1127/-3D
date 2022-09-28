#include <fstream>
#include <sstream>
#include "Model.h"


const std::string kBaseDirectory = "Resources/";

Obj* Obj::GetInstance()
{
	static Obj instance;
	return &instance;
}

bool Obj::loadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/" + filename;
	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
		return 0;
	}

	std::string line;
	std::vector<std::string> v;


	while (getline(file, line)) {

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "v") {
			// X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			temp_vertices.push_back(position);
		}

		if (key == "vn") {
			// X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			temp_normals.emplace_back(normal);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			//texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			temp_uvs.emplace_back(texcoord);
		}

		if (key == "f") {

			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// 頂点番号
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexNormal;
				Vertex vertex{};
				vertex.pos = temp_vertices[indexPosition - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexTexcoord - 1];

				obj.push_back(vertex);


			}
		}
	}
}
