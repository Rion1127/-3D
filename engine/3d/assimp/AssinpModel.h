#pragma once
#include "AssimpLoader.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "LightGroup.h"
#include <memory>
#include "Material.h"
class AssinpModel
{
public:
	AssinpModel();
	static void SetLightGroup(std::shared_ptr<LightGroup> lightGroup) { lightGroup_ = lightGroup; }
	void Create(const wchar_t* modelFile);

	void Draw(WorldTransform WT);
private:
	std::vector<Mesh> meshes_;
	std::vector<Material> materials_;
	std::vector <Texture> texture_;
	std::unique_ptr<ImportSettings> importSetting_;

	WorldTransform WorldTransform_;

	static std::shared_ptr<LightGroup> lightGroup_;
public:
	static const uint32_t MAX_BONES = 32;

	struct ConstBufferDataSkin {
		DirectX::XMMATRIX bones[MAX_BONES];
	};
};

