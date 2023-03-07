#pragma once
#include <wrl.h>
#include "ViewProjection.h"

using namespace Microsoft::WRL;
class Particle
{
public:
	Particle();
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Particle* GetInstance();

	void Ini();

	static void PreDraw();

	void Update(ViewProjection VP);


	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(WorldTransform* worldTransform, uint32_t descriptorSize);

private:
	static DirectXCommon* directX_;

	struct Vertex {
		Vector3 pos;
	};

	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap_ = nullptr;

	struct ConstBufferData {
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billBoardMat;
	};

	ComPtr<ID3D12Resource> constBuff = nullptr;
	ConstBufferData* constMatMap_ = nullptr;

	int activeCount = 1;
};

