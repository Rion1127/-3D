#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

/**
 * @file AssimpModel.cpp
 * @brief ボーンや頂点データを管理している
 */

std::shared_ptr<LightGroup> AssimpModel::lightGroup_ = nullptr;

AssimpModel::AssimpModel()
{
}

void AssimpModel::CreateModel(const std::string& fileName)
{
	fileName;
}

void AssimpModel::Init()
{

}

void AssimpModel::PlayAnimetion()
{
}

void AssimpModel::ShadowUpdate(int32_t activeNum, const Vector3& pos)
{
	lightGroup_->SetCircleShadowActive(activeNum, true);
	lightGroup_->SetCircleShadowCasterPos(activeNum, pos /*- Vector3(0, 1, 0)*/);
	lightGroup_->SetCircleShadowDir(activeNum, { 0,-1,0 });
	lightGroup_->SetCircleShadowAtten(activeNum, Vector3(0, 0, 0));
	lightGroup_->SetCircleShadowFactorAngle(activeNum, { 0.0f,1.5f });
}

void AssimpModel::Draw(const WorldTransform& WT)
{
	lightGroup_->Draw(3);
	for (auto& m : materials_) {
		m->Draw();
	}
	for (auto& v : vertices_) {
		v->Draw(WT);
	}
}
