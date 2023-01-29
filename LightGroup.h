#pragma once
#include "Vector3.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "DirectX.h"
#include <d3d12.h>
class LightGroup
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;
	static const int PointLightNum = 3;

	static void StaticInit();
	//インスタンス生成
	static LightGroup* Create();

	//初期化
	void Init();
	//定数バッファ転送
	void TransferConstBuffer();

	void SetAmbientColor(const Vector3& color);
#pragma region 平行光源
	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="acrive">有効フラグ</param>
	void SetDirLightActive(int index, bool active);
	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetDirLightDir(int index, const Vector3& lightdir);
	/// <summary>
	/// 平行光源のカラーをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetDirLightColor(int index, const Vector3& lightColor);
#pragma endregion
#pragma region 点光源
	/// <summary>
	/// 点光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="acrive">有効フラグ</param>
	void SetPointLightActive(int index, bool active);
	/// <summary>
	/// 点光源の座標をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetPointLightPos(int index, const Vector3& lightPos);
	/// <summary>
	/// 点光源のカラーをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetPointLightColor(int index, const Vector3& lightColor);
	/// <summary>
	/// 点光源の距離減衰をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetPointLightAtten(int index, const Vector3& lightAtten);
#pragma endregion
	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Draw(UINT rootParameterIndex);

private:
	//定数バッファ用データ構造体
	struct ConstBufferData {
		//環境光の色
		Vector3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//点光源用
		PointLight::ConstBufferData pointLights[PointLightNum];
	};
	static DirectXCommon* directX;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//環境光の色
	Vector3 ambientColor = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights[DirLightNum];
	//点光源の配列
	PointLight pointLights[PointLightNum];

	//ダーティフラグ
	bool dirty = false;
};

