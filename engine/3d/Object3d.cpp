#include "Object3d.h"

/**
 * @file Object3d.cpp
 * @brief 読み込んだモデルを描画するクラス
 */

Object3d::Object3d()
{
	billBoard = BillBoard::None;

	HRESULT result;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(Object3d::constBuffData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&colorBuff_));
	assert(SUCCEEDED(result));

	color_ = Color(0,0,0,0);
	Object3d::constBuffData* data;
	//定数バッファのマッピング
	result = colorBuff_->Map(0, nullptr, (void**)&data);
	assert(SUCCEEDED(result));
	data->color = color_;
	data->isUseDiffuseColor = false;
}

Object3d::~Object3d()
{
}

void Object3d::Init()
{
}

void Object3d::Update(Camera* camera)
{
	if (model_ != nullptr) {
		model_->ShadowUpdate(WT_.position_);
	}
	WT_.Update(billBoard, camera);

	HRESULT result;
	Object3d::constBuffData* data = nullptr;
	//定数バッファのマッピング
	result = colorBuff_->Map(0, nullptr, (void**)&data);
	assert(SUCCEEDED(result));
	data->color = color_;
	data->isUseDiffuseColor = isUseDiffuseColor_;
}

void Object3d::Draw()
{
	if (isVisible_)
	{
		//定数バッファビューをセット(べた書き許して)
		RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4,
			colorBuff_->GetGPUVirtualAddress());

		model_->DrawOBJ(WT_);
	}
}

void Object3d::Draw(WorldTransform WT)
{
	if (isVisible_)
	{
		model_->DrawOBJ(WT);
	}
}
