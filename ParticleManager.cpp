#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"

void ParticleManager::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetParticlePipeline(3)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->
		GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト
}

void ParticleManager::Draw()
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			DirectXCommon::GetInstance()->GetCommandList(), 0);

	DirectXCommon::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);
}
