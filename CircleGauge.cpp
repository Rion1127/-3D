#include "CircleGauge.h"

CircleGauge::CircleGauge()
{
	Init();
}

void CircleGauge::Init()
{
	sprite_.Init();

	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstRadial360Buffer) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&radianBuffer_));
	assert(SUCCEEDED(result));

	// 定数バッファマッピング
	result = radianBuffer_->Map(0, nullptr, (void**)&radianMap_);
	assert(SUCCEEDED(result));
}

void CircleGauge::Update()
{
	baseMax_ = 360.f * rate_;
	baseMin_ = 360.f - baseMax_;

	nowRadian_ = baseMin_ + baseRadian_ * rate_;

	sprite_.SetRot(angle_);
	sprite_.Update();

	//バッファにも描画範囲の回転情報を送る
	radianMap_->radian = Radian(nowRadian_);
}

void CircleGauge::Draw()
{
	sprite_.AddConstBuffDraw();
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(3, radianBuffer_->GetGPUVirtualAddress());

	sprite_.VertexDraw();
}

void CircleGauge::ImGui()
{
}
