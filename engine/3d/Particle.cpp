#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Util.h"
#include "Particle.h"
#include "DirectX.h"
#include "Texture.h"
#include "PipelineManager.h"

//コマンドリストを格納する
DirectXCommon* Particle::directX_ = nullptr;

Particle::Particle() {
	directX_ = DirectXCommon::GetInstance();

	Ini();
}

Particle* Particle::GetInstance()
{
	static Particle instance;
	return &instance;
}

void Particle::Ini()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertexCount);

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	directX_->GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	vertBuff->SetName(L"PARTICLE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff->Map(0, nullptr, (void**)&vertMap_);
	//テスト
	vertMap_->pos = { 0,0,0 };

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);


	constBuff = CreateBuff(constMatMap_);
}

void Particle::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	//パイプラインに設定した内容で描画を始める
	directX_->GetCommandList()->
		SetPipelineState(PipelineManager::GetParticlePipeline(3)->gerPipelineState());
	directX_->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // ポイントリスト
}

void Particle::Update(ViewProjection VP)
{
	constMatMap_->mat = VP.GetMatView() * VP.GetMatProjection();
	constMatMap_->billBoardMat = /*VP.matBillboard*/DirectX::XMMatrixIdentity();
}

void Particle::ChangeColor(float x, float y, float z, float w)
{
	//bVertices_.ChangeColor(x, y, z, w);
}

void Particle::ChangeColor(XMFLOAT4 color_)
{
	//bVertices_.ChangeColor(color_);
}

void Particle::Draw(WorldTransform* worldTransform,
	uint32_t descriptorSize)
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(directX_->GetCommandList(),
			descriptorSize);

	// 頂点バッファビューの設定コマンド
	directX_ ->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	//commandList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	directX_->GetCommandList()->DrawInstanced((UINT)activeCount, 1, 0, 0);
}
