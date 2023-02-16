#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"
#include <cassert>


void ParticleManager::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetParticlePipeline(3)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX_->
		GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト
}

void ParticleManager::Draw(UINT texture)
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			directX_->GetCommandList(), texture);

	directX_->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファビューをセット
	directX_->GetCommandList()
		->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	directX_->GetCommandList()->
		DrawInstanced(activeCount, 1, 0, 0);
}

void ParticleManager::Update(ViewProjection VP)
{
	HRESULT result;
	vertMap->position = { 0,0,10 };
	vertMap->color = { 1,1,1,1 };
	vertMap->scale = 5;


	/*matView = XMMatrixIdentity();
	matProjection = XMMatrixIdentity();*/
	matBillboard = XMMatrixIdentity();
	// 定数バッファへデータ転送
	result = constBuff->Map(0, nullptr, (void**)&constMap_);
	constMap_->mat = VP.GetMatView() * VP.GetMatProjection();	// 行列の合成
	constMap_->matBillboard = matBillboard;	// 行列の合成
	constBuff->Unmap(0, nullptr);
}

ParticleManager::ParticleManager()
{
	directX_ = DirectXCommon::GetInstance();

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
	result = directX_->GetDevice()
		->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}


DirectXCommon* ParticleManager::directX_ = nullptr;