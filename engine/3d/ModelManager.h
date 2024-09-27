#pragma once
#include "Model.h"
#include "AssimpModel.h"
#include <unordered_map>
class ModelManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<Model>> models_;
	std::unordered_map<std::string, std::unique_ptr<AssimpModel>> assimpModels_;
public:
	static ModelManager* GetInstance();

	Model* GetModel(std::string modelName) { return models_[modelName].get(); }
	AssimpModel* GetAssimpModel(std::string modelName) { return assimpModels_[modelName].get(); }
	//モデル初期化(CreateOBJ()に入っている)
	void LoadModel(const std::string& modelname, bool smoothing,bool isShadow, const std::string& keyName);
	void LoadAssimpModel(const std::string& fileName, const std::string& keyName);
private:
	ModelManager() {};

};

