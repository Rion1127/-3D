#include "AssimpObject3D.h"
#include "PipelineManager.h"

void AssimpObject3D::Update()
{
	worldTransform_.Update();
}

void AssimpObject3D::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetPipelineObjects("assimp")->GetPipelineStateAlpha());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects("assimp")->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void AssimpObject3D::Draw()
{
	model_->Draw(worldTransform_);
}
