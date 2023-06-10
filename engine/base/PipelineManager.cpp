#include "PipelineManager.h"

std::array<Pipeline, 4> PipelineManager::Object3dPipeline_;
std::array<Pipeline, 4> PipelineManager::SpritePipeline_;
std::array<ParticlePipeline, 4> PipelineManager::particlePipeline_;
std::array<Pipeline, 4> PipelineManager::toonPipeline_;

PipelineObject PipelineManager::posteffectPipeline_;

void PipelineManager::Ini() {
	for (int i = 0; i < 4; i++) {
		Object3dPipeline_.at(i).Object3dIni(i);
		SpritePipeline_.at(i).SpriteIni(i);
		particlePipeline_.at(i).Init(i);
		toonPipeline_.at(i).ToonIni(i);
	}


	posteffectPipeline_.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	posteffectPipeline_.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	posteffectPipeline_.Setshader(L"Resources/shader/PostEffectVS.hlsl", PipelineObject::VS);
	posteffectPipeline_.Setshader(L"Resources/shader/PostEffectPS.hlsl", PipelineObject::PS);
	//posteffectPipeline_.SetPSshader(L"Resources/shader/BasicPS.hlsl");

	posteffectPipeline_.AddrootParams(3);
	//for (int i = 0; i < 4; i++) {
		posteffectPipeline_.Init(3, PipelineObject::CULL_MODE::NONE, PipelineObject::TRIANGLE);
	//}
};