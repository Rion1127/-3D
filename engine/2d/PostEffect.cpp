#include "PostEffect.h"
#include "WinAPI.h"

PostEffect::PostEffect():Sprite()
{
	texture_ = *TextureManager::GetInstance()->GetTexture("White");
	Sprite::Ini("postEffect");
	anchorPoint_ = { 0,0 };

	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinAPI::win_width,
		(UINT)WinAPI::win_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);
	//テクスチャバッファの生成
	result =
		RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff_));
	assert(SUCCEEDED(result));

	{
		//画素数
		const auto pixelCount = WinAPI::win_width * WinAPI::win_height;
		//画像一行分のデータサイズ
		const auto rowPitch = sizeof(UINT) * WinAPI::win_width;
		//画像全体のデータサイズ
		const auto depthPitch = rowPitch * WinAPI::win_height;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}
		//テクスチャバッファにデータ転送
		result = texBuff_->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
	//SRV用でスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用でスクリプタヒープを生成
	result = RDirectX::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));
	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//デスクリプタヒープにSRV作成
	RDirectX::GetInstance()->GetDevice()->
		CreateShaderResourceView(texBuff_.Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
	isImguiDisplay = true;
}

void PostEffect::Draw()
{
	if (isImguiDisplay)
	{
		DrawImGui();
	}

	if (isInvisible_)
	{
		return;
	}
	// パイプラインステートとルートシグネチャの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(3)->gerPipelineState());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	//SRVヒープの設定コマンド
	std::vector<ID3D12DescriptorHeap*> heaps = { descHeapSRV_.Get() };
	RDirectX::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps.data());
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());
	
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
