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
};

