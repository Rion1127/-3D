#include "Field.h"
#include "ModelManager.h"

void Field::Initialize()
{
	//3Dオブジェクト
	obj_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	obj_->SetModel(ModelManager::GetInstance()->GetModel("Field"));

	//field位置
	pos_ = { 0,0,0 };
}

void Field::Update()
{
	//オブジェクトの更新
	obj_->SetPos(pos_);
	obj_->Update();
}

void Field::Draw()
{
	//オブジェクトの描画
	obj_->Draw();
}

void Field::TitleUpdate()
{
	if (rot_ >= 360) rot_ = 0;
	rot_ += 0.1f;
	//オブジェクトの更新
	obj_->SetPos(pos_);
	obj_->SetRot(Vector3(0, Radian(rot_), 0));
	obj_->Update();
}
