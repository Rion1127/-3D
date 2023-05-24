#include "Object3d.h"

LightGroup* Object3D::lightGroup = nullptr;

Object3D::Object3D()
{
	model_ = Model::CreateOBJ("Earth",true);
}

Object3D::~Object3D()
{
	delete model_;
}

void Object3D::Update()
{
	WT_.Update();
}

void Object3D::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(3)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Object3D::Draw()
{
	model_->DrawOBJ(&WT_,lightGroup);
}
