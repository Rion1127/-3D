#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>
#include "ViewProjection.h"
class ParticleManager
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void PreDraw();
	void Draw(UINT texture);

	void Update(ViewProjection VP);

	ParticleManager();

	~ParticleManager() {
		// �}�b�v������
		vertBuff->Unmap(0, nullptr);
	};

	struct Vertex {
		Vector3 position;
		float scale;
		DirectX::XMFLOAT4 color;
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;	// �R�c�ϊ��s��
		DirectX::XMMATRIX matBillboard;
	};
protected:
	static DirectXCommon* directX_;

	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;

	

	int activeCount = 5;

	uint32_t texture_ = 0;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matProjection;
	DirectX::XMMATRIX matBillboard;
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	ConstBufferData* constMap_;
};

