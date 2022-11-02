#include "Vector3.h"
#include "RailCamera.h"

void RailCamera::Ini(Vector3 pos,Vector3 rot)
{
	//ワールドトランスフォーム初期化
	worldTransform.position = pos;
	worldTransform.rotation = rot;
	//ビュープロジェクション初期化
	viewProjection.Ini();
}

void RailCamera::Update()
{
	worldTransform.AddPosition(0, 0, 0);
	worldTransform.AddRotation(0, 0, 0);
	worldTransform.Update(viewProjection);

	viewProjection.eye.x = worldTransform.position.x;
	viewProjection.eye.y = worldTransform.position.y;
	viewProjection.eye.z = worldTransform.position.z;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = worldTransform.rotation;
	//視点から前方に適当な距離進んだ位置が中止店
	viewProjection.target.x = viewProjection.eye.x + forward.x;
	viewProjection.target.y = viewProjection.eye.y + forward.y;
	viewProjection.target.z = viewProjection.eye.z + forward.z;
	//ワールド上方向ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映（レールカメラの上方向ベクトル）
	viewProjection.up.x = worldTransform.rotation.x;
	viewProjection.up.y = worldTransform.rotation.y;
	viewProjection.up.z = worldTransform.rotation.z;
	//ビュープロジェクションの更新
	viewProjection.Update();
}
