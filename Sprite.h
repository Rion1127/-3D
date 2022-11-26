#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini();
	//座標を代入する
	void SetPos(Vector2 pos);
	//回転させる
	void SetRot(float rot);
	//スケール設定
	void SetScale(Vector2 scale);

	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(DirectX::XMFLOAT4 color) {
		color_ = color;
	};

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void PreDraw();
	//画像サイズ自動取得(描画座標は中心)
	void Draw(UINT descriptorSize);
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
private:
	DirectXCommon* directX_;
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
	//////////////////////////後でクラス化
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	////////////////////////////

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//定数バッファ用データ構造体
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	XMMATRIX matProjection;

	// ワールド行列
	DirectX::XMMATRIX matWorld_{};
	//色
	DirectX::XMFLOAT4 color_;
	//回転
	float rot_;
	//座標
	Vector2 pos_;
	//スケール
	Vector2 Scale_;
};

