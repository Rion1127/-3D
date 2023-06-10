#pragma once
#include "Vector3.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "DirectX.h"
#include <d3d12.h>
class LightGroup
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const uint32_t DirLightNum = 3;
	static const uint32_t PointLightNum = 3;
	static const uint32_t SpotLightNum = 3;
	static const uint32_t CircleShadowNum = 1;

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
	void SetDirLightActive(uint32_t index, bool active);
	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetDirLightDir(uint32_t index, const Vector3& lightdir);
	/// <summary>
	/// 平行光源のカラーをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetDirLightColor(uint32_t index, const Vector3& lightColor);
#pragma endregion
#pragma region 点光源
	/// <summary>
	/// 点光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="acrive">有効フラグ</param>
	void SetPointLightActive(uint32_t index, bool active);
	/// <summary>
	/// 点光源の座標をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetPointLightPos(uint32_t index, const Vector3& lightPos);
	/// <summary>
	/// 点光源のカラーをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetPointLightColor(uint32_t index, const Vector3& lightColor);
	/// <summary>
	/// 点光源の距離減衰をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト色</param>
	void SetPointLightAtten(uint32_t index, const Vector3& lightAtten);
#pragma endregion
#pragma region スポットライト
	void SetSpotLightActive(uint32_t index, bool active);
	void SetSpotLightDir(uint32_t index, const Vector3& lightdir);
	void SetSpotLightPos(uint32_t index, const Vector3& lightPos);
	void SetSpotLightColor(uint32_t index, const Vector3& lightColor);
	void SetSpotLightAtten(uint32_t index, const Vector3& lightAtten);
	void SetSpotLightFactorAngle(uint32_t index, const Vector2& lightFactorAngle);
#pragma endregion
#pragma region 丸影
	void SetCircleShadowActive(uint32_t index, bool active);
	void SetCircleShadowCasterPos(uint32_t index, const Vector3& casterPos);
	void SetCircleShadowDir(uint32_t index, const Vector3& lightdir);
	void SetCircleShadowDistanceCasterLight(uint32_t index, float ditanceCasterLight);
	void SetCircleShadowAtten(uint32_t index, const Vector3& lightAtten);
	void SetCircleShadowFactorAngle(uint32_t index, const Vector2& lightFactorAngle);
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
		//点光源用
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//丸影
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};
	static RDirectX* directX;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//環境光の色
	Vector3 ambientColor = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights[DirLightNum];
	//点光源の配列
	PointLight pointLights[PointLightNum];
	//スポットライト
	SpotLight spotLights[SpotLightNum];
	//丸影
	CircleShadow circleShadows[CircleShadowNum];

	//ダーティフラグ
	bool dirty = false;
};

