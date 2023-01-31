#pragma once
#include <wrl.h>
#include "Vertices.h"
using namespace Microsoft::WRL;
class BoardObject
{
public:
	BoardObject();
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static BoardObject* GetInstance();

	static void Ini();

	void shadowIni();

	static void PreDraw();

	void shadowPreDraw();

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(WorldTransform* worldTransform, uint32_t descriptorSize);

	void ShadowDraw(WorldTransform* worldTransform, uint32_t descriptorSize);

private:
	

	// ルートシグネチャ
	static ComPtr < ID3D12RootSignature> sRootSignature;
	// パイプランステートの生成
	static ComPtr<ID3D12PipelineState> sPipelineState;

	//コマンドリストを格納する
	static DirectXCommon* sdirectX_ ;
	//頂点データ
	static Vertices sVertices_;
};

