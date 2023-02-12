#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>

class ParticleManager
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void PreDraw();
	void Draw();

	struct Vertex {
		Vector3 position;
		float scale;
		DirectX::XMFLOAT4 color;
	};
protected:
	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;

	int activeCount = 0;
};

