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
	//定数バッファ0番
	entity[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	entity[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	entity[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	entity[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	////テクスチャレジスタ0番
	entity[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	entity[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	entity[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	entity[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番
	entity[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	entity[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	entity[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	entity[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
}
