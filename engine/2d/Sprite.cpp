#include <d3dcompiler.h>

#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include <string>
#include "Texture.h"
#include <DirectXTex.h>
#include "Util.h"
#include "Sprite.h"
#include <sstream>
#include "myMath.h"
#include "Camera2d.h"

/**
 * @file Sprite.cpp
 * @brief 2Dスプライトに関するクラス
 */

uint32_t Sprite::SAllNum = 0;

Sprite::Sprite(const std::string& name)
{
	Init(name);
}

void Sprite::Init(const std::string& name)
{
	if (RDirectX::GetInstance()->GetDevice() == nullptr)return;
	spriteNum_ = SAllNum;
	Sprite::AddAllNum();
	name;

	//std::string noneString = "";
	////何も入っていない場合
	//if (guiname == noneString) {
	//	std::ostringstream oss;
	//	oss << spriteNum_;
	//	name_ = "Sprite" + oss.str();
	//	gui_ = name_.c_str();
	//}
	//else {
	//	guiName_ = guiname;
	//	gui_ = guiName_.c_str();
	//}

	HRESULT result;
	vertex_.Init();
#pragma region シェーダーに色を渡す

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);
	assert(SUCCEEDED(result));
	//値を書き込むと自動的に転送される
	constMapMaterial_->color = Color(1, 1, 1, 1.0f);
#pragma endregion

#pragma region トランスフォーム
	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result));
	}

	// 定数バッファマッピング
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result));
	//単位行列を代入
	constMapTransform_->mat.UnitMatrix();

	// 射影行列計算
	matProjection_ = {
		2 / WinAPI::GetWindowSize().x,0,0,0,
		0,-2 / WinAPI::GetWindowSize().y,0,0,
		0,0,1,0,
		-1,1,0,1
	};
#pragma endregion

	rot_ = 0;
	//スケール
	scale_ = { 1.f,1.f };
	anchorPoint_ = { 0.5f,0.5f };
	splitNum_ = { 1,1 };
	tilingNum_ = { 1,1 };

	isMatrixChange_ = false;
}

void Sprite::Update()
{
	if (isVisible_ == false)
	{
		return;
	}
	//画像サイズを指定している場合スケールを自動で変換する
	if (textureSize_.x != 0 && textureSize_.y != 0)
	{
		if (isScaleChange_ == false) {
			scale_ = {
				1.f / splitNum_.x,
				1.f / splitNum_.y,
			};
		}
		isScaleChange_ = false;
	}
	//セッター関数で行列を変更していた場合計算しない
	if (isMatrixChange_ == false) {
		// ワールド行列の更新
		float zoomRate = Camera2d::GetInstance()->GetZoomRate();
		matWorld_.UnitMatrix();
		matWorld_ *= ConvertScalingMat({ scale_.x * zoomRate, scale_.y * zoomRate, 0 });
		matWorld_ *= ConvertRotationZAxisMat(rot_);
		matWorld_ *= ConvertTranslationMat({ pos_.x * zoomRate, pos_.y * zoomRate, 0.0f });
	}
	//毎フレームfalseに戻す
	else {
		isMatrixChange_ = false;
	}

	// 定数バッファにデータ転送
	constMapMaterial_->color = color_ / 255.f;
	constMapTransform_->mat = matWorld_ * matProjection_; // 行列の合成
	constMapTransform_->tiling = tilingNum_; // 行列の合成

#pragma endregion

	TransferVertex();
}

void Sprite::Draw()
{
	if (isVisible_ == false) {
		return;
	}

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	vertex_.Draw();
}

void Sprite::Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY)
{
	if (isVisible_ == false) {
		return;
	}
	//画像の頂点データを更新
	vertex_.SetVerticesPos(LuX, LuY, RuX, RuY, LdX, LdY, RdX, RdY);

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	vertex_.Draw();
}

void Sprite::Draw(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB)
{
	Draw(LT.x, LT.y, RT.x, RT.y, LB.x, LB.y, RB.x, RB.y);
}

void Sprite::AddConstBuffDraw()
{
	if (isVisible_ == false) {
		return;
	}

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
}

void Sprite::VertexDraw()
{
	vertex_.Draw();
}

void Sprite::SetSprite_Size(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB)
{
	isVertChange = true;
	vertex_.SetVerticesPos(LT, RT, LB, RB);
}

void Sprite::TransferVertex()
{
	float left = (0.0f - anchorPoint_.x) * texture_.size_.x;
	float right = (1.0f - anchorPoint_.x) * texture_.size_.x;
	float top = (0.0f - anchorPoint_.y) * texture_.size_.y;
	float bottom = (1.0f - anchorPoint_.y) * texture_.size_.y;
	//左右反転
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	if (isVertChange == false)
	{
		vertex_.SetVerticesPos(left, right, top, bottom);
	}
	isVertChange = false;

	//切り取り範囲がどちらも0の場合UV座標は変えない
	if (textureSize_.x != 0 && textureSize_.y != 0)
	{
		float tex_left = textureLeftTop_.x / texture_.size_.x;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / texture_.size_.x;
		float tex_top = textureLeftTop_.y / texture_.size_.y;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / texture_.size_.y;
		//頂点のUVに反映する
		vertex_.SetVerticesUv(tex_left, tex_right, tex_top, tex_bottom);
	}
	
	vertex_.TransferVertex();
}
