#include "mRootParameter.h"

void mRootParameter::Ini()
{
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1;					//一度の描画に好かうテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;				//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートのパラメータ設定
	D3D12_ROOT_PARAMETER rootparam;
	//定数バッファ0番
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootparam.Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootparam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	entity.push_back(rootparam);
	////テクスチャレジスタ0番
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	rootparam.DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	entity.push_back(rootparam);
	//定数バッファ1番
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootparam.Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootparam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	entity.push_back(rootparam);
}
