#pragma once
#include <wrl.h>
#include "Vertices.h"
using namespace Microsoft::WRL;
class Object3d
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Object3d* GetInstance();

	static void Ini(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	void ChangeColor(float x,float y,float z,float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(WorldTransform* worldTransform,uint32_t descriptorSize);

private:
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_;
	
	
};

