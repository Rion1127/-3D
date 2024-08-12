#pragma once
#include "Vector3.h"
#include "Color.h"
#include "myMath.h"
#include "d3d12.h"
#include <wrl.h>
#include <vector>

class SpriteVertex
{
public:
	void Init();
	void TransferVertex();

	void Draw();

public:
	//頂点座標
	//左右上下を指定する
	void SetVerticesPos(float left,float right, float top, float bottom);
	//全ての頂点を細かく指定する
	void SetVerticesPos(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY);
	void SetVerticesPos(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB);
	//UV座標
	//左右上下を指定する
	void SetVerticesUv(float left, float right, float top, float bottom);
	//全ての頂点を細かく指定する
	void SetVerticesUv(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY);
private:
	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};
	enum VertNum {
		LB,	//左下
		LT,	//左上
		RB,	//右下
		RT	//右上
	};
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	const int32_t vertnum = 4;

	//頂点データ
	std::vector<Vertex> vertices_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	//インデックスデータ
	std::vector<uint16_t> indices_;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
private:
	void Init_Vertices();
	void Init_Indices();
};

