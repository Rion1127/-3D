#pragma once
#include "AssimpLoader.h"
#include "WorldTransform.h"
#include "Texture.h"
class AssinpModel
{
public:
	AssinpModel();
	void Create(const wchar_t* modelFile);

	void Draw(WorldTransform WT);
private:
	std::vector<Mesh> meshes_;
	std::vector <Texture> texture_;
	std::unique_ptr<ImportSettings> importSetting_;

	


	WorldTransform WorldTransform_;
public:
	static const uint32_t MAX_BONES = 32;

	struct ConstBufferDataSkin {
		DirectX::XMMATRIX bones[MAX_BONES];
	};
};

