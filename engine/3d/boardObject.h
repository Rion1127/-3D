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

	static void PreDraw();


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
	Vertex* vertMap = nullptr;

};

