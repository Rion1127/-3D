#include "LightGroup.h"
#include <cassert>

DirectXCommon* LightGroup::directX = nullptr;

void LightGroup::StaticInit()
{
	directX = DirectXCommon::GetInstance();
}

LightGroup* LightGroup::Create()
{
	//3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	//初期化
	instance->Init();

	return instance;
}

void LightGroup::Init()
{
	//標準のライトの設定
	DefaultLightSetting();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// 定数バッファの生成
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファのマッピング
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++) {
			//ライトが有効なら設定を転送
			if (dirLights[i].IsAvtive()) {
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			//ライトが無効なら転送しない
			else {
				constMap->dirLights[i].active = false;
			}
		}


		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const Vector3& lightColor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightColor);
	dirty = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[0].SetLightDir({ 0.0f,-1.0f,0.0f });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f });
}

void LightGroup::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	//定数バッファビューをセット
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
