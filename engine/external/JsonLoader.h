#pragma once
#include <string>
#include "Object3D.h"
#include "json.hpp"

struct LevelData {
	std::vector<std::unique_ptr<Object3d>> object;
	std::vector<std::string> fileName;
};
class JsonLoader
{
private:
	std::string kDefaultBaseDirectory;

	std::vector<std::unique_ptr<LevelData>> levelData_;
public:
	static JsonLoader* GetInstance();
	JsonLoader();


	void LoadFile(std::string fileName);
	void SetObjects(std::vector<std::unique_ptr<Object3d>>* objects, int dataNum) {

		LevelData* data = levelData_.at(dataNum).get();
		size_t num = data->object.size();

		for (size_t i = 0; i < num; i++)
		{
			//ì«Ç›çûÇÒÇæèÓïÒÇë„ì¸ÇµÇƒÇ≠
			std::unique_ptr<Object3d> newObj = std::move(std::make_unique<Object3d>());
			newObj->SetPos(data->object.at(i)->GetPos());
			newObj->SetRot(data->object.at(i)->GetRot());
			newObj->SetScale(data->object.at(i)->GetScale());
			objects->push_back(std::move(newObj));
		}
	}
private:
	
private:
	//ÉQÉbÉ^Å[
	//std::vector<std::unique_ptr<LevelData>> GetLevelData() { return levelData_; }

};

