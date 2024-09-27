#include "ModelManager.h"
#include "AssimpLoader.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadModel(const std::string& modelname, bool smoothing, bool isShadow, const std::string& keyName)
{
	std::unique_ptr<Model> instance = std::make_unique<Model>();
	instance = Model::CreateOBJ_uniptr(modelname, smoothing, isShadow);

	models_.insert(std::make_pair(keyName, std::move(instance)));
}

void ModelManager::LoadAssimpModel(const std::string& fileName, const std::string& keyName)
{
	auto model = AssimpLoader::GetInstance()->Load(fileName);

	assimpModels_.insert(std::make_pair(keyName, std::move(model)));
}
