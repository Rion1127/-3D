#include "Object3D.h"
#include "DirectX.h"

Object3D::Object3D()
{
	model = Model::CreateOBJ("sphere",true);
}

Object3D::~Object3D()
{
	delete model;
}

void Object3D::Init()
{
}

void Object3D::Update()
{
	worldTrnasform_.Update();
}
void Object3D::SetBlend(int blend)
{
	if (blend > 3) blend = 3;
	else if (blend < 0) blend = 0;
	// パイプラインステートとルートシグネチャの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(blend)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(blend)->GetRootSignature());
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
	model->DrawOBJ(&worldTrnasform_);
}
