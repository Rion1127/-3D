#pragma once
#include <wrl.h>
#include <string>
#include "Vertices.h"
using namespace Microsoft::WRL;
class Object3d
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Object3d();

	static Object3d* GetInstance();

	static void Ini(ID3D12Device* device);

	static Object3d* CreateOBJ(const std::string& modelname, ID3D12Device* device);
	//.OBJから情報を読み込む
	void LoadOBJ(const std::string& modelname);

	void ModelIni(const std::string& modelname, ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	void ChangeColor(float x,float y,float z,float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(WorldTransform* worldTransform,uint32_t descriptorSize);

	void Draw2(WorldTransform* worldTransform, uint32_t textureHandle);

private:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//uv座標
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_;
	
	std::vector<Vertices*> vert_;
};

