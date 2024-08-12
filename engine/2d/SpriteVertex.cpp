#include "SpriteVertex.h"
#include "DirectX.h"
#include <cassert>

void SpriteVertex::Init()
{
	Init_Vertices();
	Init_Indices();
}

void SpriteVertex::TransferVertex()
{
	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);
}

void SpriteVertex::Draw()
{
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void SpriteVertex::SetVerticesPos(float left, float right, float top, float bottom)
{
	vertices_.at(LB).pos = { left	, bottom	,0 };//左下
	vertices_.at(LT).pos = { left	, top		,0 };//左上
	vertices_.at(RB).pos = { right	, bottom	,0 };//右下
	vertices_.at(RT).pos = { right	, top		,0 };//右上
}

void SpriteVertex::SetVerticesPos(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY)
{
	vertices_.at(LB).pos = { LdX,LdY,0 };//左下
	vertices_.at(LT).pos = { LuX,LuY,0 };//左上
	vertices_.at(RB).pos = { RdX,RdY,0 };//右下
	vertices_.at(RT).pos = { RuX,RuY,0 };//右上
}

void SpriteVertex::SetVerticesPos(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB)
{
	vertices_.at(VertNum::LB).pos = { LB.x,LB.y,0 };//左下
	vertices_.at(VertNum::LT).pos = { LT.x,LT.y,0 };//左上
	vertices_.at(VertNum::RB).pos = { RB.x,RB.y,0 };//右下
	vertices_.at(VertNum::RT).pos = { RT.x,RT.y,0 };//右上
}

void SpriteVertex::SetVerticesUv(float left, float right, float top, float bottom)
{
	//頂点のUVに反映する
	vertices_.at(LB).uv = { left	, bottom };//左下
	vertices_.at(LT).uv = { left	, top };//左上
	vertices_.at(RB).uv = { right	, bottom };//右下
	vertices_.at(RT).uv = { right	, top };//右上
}

void SpriteVertex::SetVerticesUv(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY)
{
	vertices_.at(LB).uv = { LdX,LdY };//左下
	vertices_.at(LT).uv = { LuX,LuY };//左上
	vertices_.at(RB).uv = { RdX,RdY };//右下
	vertices_.at(RT).uv = { RuX,RuY };//右上
}

void SpriteVertex::Init_Vertices()
{
	HRESULT result;
	//頂点データ
	vertices_.resize(vertnum);
	vertices_.at(LB) = { {   -0.0f,100.0f,0.0f },{0.0f,1.0f} };//左下
	vertices_.at(LT) = { {   -0.0f, +0.0f,0.0f },{0.0f,0.0f} };//左上
	vertices_.at(RB) = { { +100.0f,100.0f,0.0f },{1.0f,1.0f} };//右下
	vertices_.at(RT) = { { +100.0f, +0.0f,0.0f },{1.0f,0.0f} };//右上

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

void SpriteVertex::Init_Indices()
{
	HRESULT result;
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(3);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	indexBuff_->Unmap(0, nullptr);
	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}
