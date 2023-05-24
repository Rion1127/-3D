#include "JsonLoader.h"
#include "json.hpp"
#include <fstream>
#include <cassert>

JsonLoader* JsonLoader::GetInstance()
{
	static JsonLoader instance;
	return &instance;
}

JsonLoader::JsonLoader()
{
	kDefaultBaseDirectory = "Resources/JSON/";
}

void JsonLoader::LoadFile(std::string fileName)
{
	//連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName;

	//ファイルストリーム
	std::ifstream file;
	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//ただ仕入れ別データファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	

}
