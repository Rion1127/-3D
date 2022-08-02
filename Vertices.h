#pragma once
#include <wrl.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include <vector>
#include "Material.h"

class Vertices
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(
		ID3D12GraphicsCommandList* commandList,
		WorldTransform* worldTransform,
		UINT descriptorSize);

private:
	struct Vertex {
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//uv座標
	};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//頂点データ
	std::vector<Vertex> vertices;
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//頂点インデックス
	std::vector<uint16_t> indices;

	Material material;
};

