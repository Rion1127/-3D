#include "PipelineManager.h"

std::map<std::string, std::unique_ptr<PipelineObject>> PipelineManager::pipelineObjects_;

void PipelineManager::Ini() {
	//オブジェクト3D
	AddPipeline("Object3D");
	GetPipelineObjects("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Object3D")->Setshader(L"Resources/shader/BasicVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("Object3D")->Setshader(L"Resources/shader/BasicPS.hlsl", PipelineObject::PS);

	GetPipelineObjects("Object3D")->AddrootParams(4);

	Create("Object3D", BACK,TOPOLOGY_TRIANGLE,DEPTH_WRITE_MASK_ALL,MODE_WRAP);
	//スプライト
	AddPipeline("Sprite");
	GetPipelineObjects("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Sprite")->Setshader(L"Resources/shader/SpriteVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("Sprite")->Setshader(L"Resources/shader/SpritePS.hlsl", PipelineObject::PS);

	GetPipelineObjects("Sprite")->AddrootParams(3);

	Create("Sprite", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);
	
	//トゥーンオブジェクト3D
	AddPipeline("Toon");
	GetPipelineObjects("Toon")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Toon")->Setshader(L"Resources/shader/ToonVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("Toon")->Setshader(L"Resources/shader/ToonPS.hlsl", PipelineObject::PS);

	GetPipelineObjects("Toon")->AddrootParams(4);

	Create("Toon", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//オブジェクト3D
	AddPipeline("assimp");
	GetPipelineObjects("assimp")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("assimp")->Setshader(L"Resources/shader/assimpVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("assimp")->Setshader(L"Resources/shader/assimpPS.hlsl", PipelineObject::PS);

	GetPipelineObjects("assimp")->AddrootParams(4);

	Create("assimp", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//ポストエフェクト
	AddPipeline("PostEffect");
	GetPipelineObjects("PostEffect")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("PostEffect")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("PostEffect")->Setshader(L"Resources/shader/PostEffectVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("PostEffect")->Setshader(L"Resources/shader/PostEffectPS.hlsl", PipelineObject::PS);

	GetPipelineObjects("PostEffect")->AddrootParams(2);

	Create("PostEffect", NONE, TOPOLOGY_TRIANGLE,DEPTH_WRITE_MASK_ZERO,MODE_BORDER);

	//ポストエフェクト
	AddPipeline("MultiTexture");
	GetPipelineObjects("MultiTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiTexture")->Setshader(L"Resources/shader/MultiTextureVS.hlsl", PipelineObject::VS);
	GetPipelineObjects("MultiTexture")->Setshader(L"Resources/shader/MultiTexturePS.hlsl", PipelineObject::PS);

	GetPipelineObjects("MultiTexture")->AddrootParamsMultiTexture(2,1);

	Create("MultiTexture", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);

}
void PipelineManager::Create(
	const std::string& pipelinename, CULL_MODE cullmode,
	TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
	TEXTURE_ADDRESS_MODE uvMode)
{
	for (size_t i = 0; i < 4; i++) {
		pipelineObjects_[pipelinename]->
			Create(BlendNum(i), cullmode, topologytype, depthWriteMasc, uvMode);
	}
}
void PipelineManager::AddPipeline(const std::string& pipelinename)
{
	std::unique_ptr<PipelineObject> obj = std::move(std::make_unique<PipelineObject>());
	obj->name_ = pipelinename;
	pipelineObjects_.insert(std::make_pair(pipelinename, std::move(obj)));
};

