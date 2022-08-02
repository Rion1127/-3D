#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
class Object2D
{
public:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void PreDraw(ID3D12GraphicsCommandList* commandList);
	//画像サイズ自動取得
	void Draw(Vector2 pos,UINT descriptorSize);
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
private:
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

#pragma region 頂点データ
	//頂点データ
	std::vector<Vertex> vertices;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;
#pragma endregion
#pragma region インデックスデータ
	//インデックスデータ
	std::vector<uint16_t> indices;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion


	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//定数バッファ用データ構造体
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

