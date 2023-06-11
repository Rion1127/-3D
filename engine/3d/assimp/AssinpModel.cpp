#include "AssinpModel.h"
#include "Texture.h"
#include "DirectX.h"

std::shared_ptr<LightGroup> AssinpModel::lightGroup_ = nullptr;

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
	materials_.resize(meshes_.size());
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer();

		std::string texturename = WStringToString(meshes_[i].diffuseMap);
		TextureManager::GetInstance()->LoadGraph(texturename, texturename);
		texture_[i] = *TextureManager::GetInstance()->GetTexture(texturename);

		//マテリアル生成
		//マテリアルの値を代入する（現在は適当な値を入れている）
		materials_[i].SetAmbient({ 0.8f,0.8f,0.8f });
		materials_[i].SetDiffuse({ 0.3f,0.3f,0.3f });
		materials_[i].SetSpecular({ 0.3f,0.3f,0.3f });

		materials_[i] = *Material::Create(RDirectX::GetInstance()->GetDevice());
		materials_[i].SetTexture(*TextureManager::GetInstance()->GetTexture(texturename));

	}
}

void AssinpModel::Draw(WorldTransform WT)
{
	lightGroup_->Draw(3);
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		/*TextureManager::GetInstance()->
			SetGraphicsDescriptorTable(texture_[i].textureHandle);*/
		materials_[i].Draw(texture_[i].textureHandle);

		importSetting_->meshes[i].Vertices.Draw(&WT, 0);
	}
}

