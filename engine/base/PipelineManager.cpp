#include "PipelineManager.h"

std::map<std::string, std::unique_ptr<PipelineObject>> PipelineManager::pipelineObjects_;

void PipelineManager::Ini() {
	//オブジェクト3D
	AddPipeline("Object3D");
	GetPipelineObjects("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Object3D")->Setshader(L"Resources/shader/BasicVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Object3D")->Setshader(L"Resources/shader/BasicPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Object3D")->AddrootParams(4);

	Create("Object3D", BACK,TOPOLOGY_TRIANGLE,DEPTH_WRITE_MASK_ALL,MODE_WRAP);
	//スプライト
	AddPipeline("Sprite");
	GetPipelineObjects("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Sprite")->Setshader(L"Resources/shader/SpriteVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Sprite")->Setshader(L"Resources/shader/SpritePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Sprite")->AddrootParams(3);

	Create("Sprite", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);
	
	//トゥーンオブジェクト3D
	AddPipeline("Toon");
	GetPipelineObjects("Toon")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Toon")->Setshader(L"Resources/shader/ToonVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Toon")->Setshader(L"Resources/shader/ToonPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Toon")->AddrootParams(4);

	Create("Toon", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//オブジェクト3D
	AddPipeline("assimp");
	GetPipelineObjects("assimp")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("assimp")->Setshader(L"Resources/shader/assimpVS.hlsl", ShaderType::VS);
	GetPipelineObjects("assimp")->Setshader(L"Resources/shader/assimpPS.hlsl", ShaderType::PS);

	GetPipelineObjects("assimp")->AddrootParams(4);

	Create("assimp", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//ポストエフェクト
	AddPipeline("PostEffect");
	GetPipelineObjects("PostEffect")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("PostEffect")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("PostEffect")->Setshader(L"Resources/shader/PostEffectVS.hlsl", ShaderType::VS);
	GetPipelineObjects("PostEffect")->Setshader(L"Resources/shader/PostEffectPS.hlsl", ShaderType::PS);

	GetPipelineObjects("PostEffect")->AddrootParams(2);

	Create("PostEffect", NONE, TOPOLOGY_TRIANGLE,DEPTH_WRITE_MASK_ZERO,MODE_BORDER);

	//ポストエフェクト
	AddPipeline("MultiTexture");
	GetPipelineObjects("MultiTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiTexture")->Setshader(L"Resources/shader/MultiTextureVS.hlsl", ShaderType::VS);
	GetPipelineObjects("MultiTexture")->Setshader(L"Resources/shader/MultiTexturePS.hlsl", ShaderType::PS);

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
}
void PipelineManager::PreDraw(std::string pipelinename, TopologyName topologyName)
{
	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList.SetPipelineState(
		PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateAlpha());

	cmdList.SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects(pipelinename)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList.IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topologyName); // 三角形リスト
}


