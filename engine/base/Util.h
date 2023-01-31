#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"

//WARNINGの詳細表示
void DisplayWarningInfo(ID3D12Device* device);

#pragma region シェーダー読み込み
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string型からwchara_t型に変換する
wchar_t* ConvertStrToWchar(const std::string string);

// 現在の時刻をミリ秒単位で取得
int GetNowCount(void);

/// <summary>
/// 引数にいれた時間からの経過時間を返す
/// </summary>
/// <param name="startCount"> 開始時間 </param>
/// <returns></returns>
float GetNowTime(int startCount);

/// <summary>
	/// 入れた値が最大値を上回っていたら最大値を返し、最小値を下回っていたら最小値を返す。
	/// どちらにも当てはまらない場合は、値をそのまま返す
	/// </summary>
	/// <param name="value"> 値 </param>
	/// <param name="max"> 最大値 </param>
	/// <param name="min"> 最小値 </param>
	/// <returns></returns>
float Clamp(float value, float max, float min);

void MoveTo(Vector3 goal, float speed,WorldTransform& WT);

Vector3 MoveTo(Vector3 goal, float speed, DirectX::XMFLOAT3& WT);
