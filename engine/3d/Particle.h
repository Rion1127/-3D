#pragma once
#include <wrl.h>
#include "Camera.h"

using namespace Microsoft::WRL;
class Particle
{
public:
	Particle();
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Particle* GetInstance();

	void Ini();

	static void PreDraw();

	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(uint32_t blend);

	void Update(Camera VP);


	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(const XMFLOAT4& color_);

	void Draw(uint32_t descriptorSize);

private:
	static RDirectX* directX_;

	struct Vertex {
		Vector3 pos;
		float scale;
		DirectX::XMFLOAT4 color;
	};

	const uint32_t CvertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	struct ConstBufferData {
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billBoardMat;
	};

	ComPtr<ID3D12Resource> constBuff_ = nullptr;
	ConstBufferData* constMatMap_ = nullptr;

	uint32_t activeCount_ = 2;
};

