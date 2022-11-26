
#include "Util.h"
#include "WorldTransform.h"

WorldTransform::WorldTransform()
{
	
}

void WorldTransform::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
}

void WorldTransform::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
}

void WorldTransform::SetPosition(float x, float y, float z)
{
	position = { x,y,z };
}

void WorldTransform::AddScale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void WorldTransform::Ini()
{
	HRESULT result;

	directx_ = DirectXCommon::GetInstance();

	//定数バッファのヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = directx_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
	

	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void WorldTransform::Update(ViewProjection viewProjection, int isBillboard)
{
	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	//ビルボード
	if (isBillboard == 1) {
		matWorld *= viewProjection.matBillboard;
	}
	else if (isBillboard == 2) {
		matWorld *= viewProjection.matBillboardY;
	}

	matWorld *= matScale;			//ワールド行列にスケーリングを反映
	matWorld *= matRot;				//ワールド行列に開店を反映
	matWorld *= matTrans;			//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent) {
		//親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	//定数バッファへデータ転送
	constMapTransform->mat = matWorld ;
	constMapTransform->viewProj = viewProjection.GetMatView() * viewProjection.GetMatProjection();
}

