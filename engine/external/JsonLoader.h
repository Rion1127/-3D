#pragma once
#include <string>
#include "Object3D.h"
#include "json.hpp"
#include "Camera.h"
struct CameraInfo {
	Vector3 pos;
	Vector3 rot;
};

struct LevelData {
	std::vector<std::unique_ptr<Object3d>> object;
	CameraInfo cameraInfo;
};
class JsonLoader
{
private:
	std::string kDefaultBaseDirectory;

	std::map<std::string ,std::unique_ptr<LevelData>> levelData_;
public:
	static JsonLoader* GetInstance();
	JsonLoader();

	//JSONファイル読み込み dataNameで名前をつける
	void LoadFile(std::string fileName, std::string dataName);
	void SetObjects(std::vector<std::unique_ptr<Object3d>>* objects, std::string levelDataName);
	void SetCamera(Camera* camera, std::string levelDataName);
private:
	
private:
	//ゲッター
	//std::vector<std::unique_ptr<LevelData>> GetLevelData() { return levelData_; }

};

