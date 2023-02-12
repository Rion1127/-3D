#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"
#include <cassert>

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
	HRESULT result;
	vertMap->position = { 0,0,0 };
	vertMap->color = { 1,1,1,1 };
	vertMap->scale = 2;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matView * matProjection;	// 行列の合成
	constMap->matBillboard = matBillboard;	// 行列の合成
	constBuff->Unmap(0, nullptr);


	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			DirectXCommon::GetInstance()->GetCommandList(), texture_);

	DirectXCommon::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);

	DirectXCommon::GetInstance()->GetCommandList()->
		DrawInstanced(activeCount,1,0,0);
}

ParticleManager::ParticleManager()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 1);

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

	DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	vertBuff->SetName(L"PARTICLE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()
		->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}
