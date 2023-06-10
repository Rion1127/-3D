#include "AssinpModel.h"
#include "Texture.h"
#include "DirectX.h"

AssinpModel::AssinpModel()
{
}

void AssinpModel::Create(const wchar_t* modelFile)
{
	ImportSettings importSetting = {
		modelFile,
		meshes_,
		false,
		true
	};

	importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));

	AssimpLoader::GetInstance()->Load(importSetting_.get());

	texture_.resize(meshes_.size());
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer();

		std::string texturename = WStringToString(meshes_[i].diffuseMap);
		TextureManager::GetInstance()->LoadGraph(texturename, texturename);
		texture_[i] = *TextureManager::GetInstance()->GetTexture(texturename);
	}
}

void AssinpModel::Draw(WorldTransform WT)
{
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		TextureManager::GetInstance()->
			SetGraphicsDescriptorTable(0);

		importSetting_->meshes[i].Vertices.Draw(&WT, 0);
	}
}

