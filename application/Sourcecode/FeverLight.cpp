#include "FeverLight.h"
#include <imgui.h>
#include "ModelManager.h"
#include "PipelineManager.h"
#include "mInput.h"
#include "Easing.h"
#include "RRandom.h"
#include "Util.h"

void FeverLight::Init()
{
	feverPos_ = { 0,80,-70 };
	defaultPos_ = { 0,95,-70 };
	pos_ = defaultPos_;
	flower_ = std::make_unique<Object3d>();
	flower_->SetModel(ModelManager::GetInstance()->GetModel("Flower"));
	flower_->SetPos(defaultPos_);
	flower_->SetIsUseDiffuseColor(true);

	rot_ = { Radian(180),0,0 };
	flower_->SetRot(rot_);
	lightDefaultScale_ = Vector3(1.5f, 7, 1.5f);
	for (int32_t i = 0; i < 10;i++) {
		lights_.emplace_back();
		auto& light = lights_.back();
		light.obj = std::make_unique<Object3d>();
		light.obj->SetModel(ModelManager::GetInstance()->GetModel("Light"));
		light.obj->SetParent(flower_->GetTransform());
		light.obj->SetScale(lightDefaultScale_);

		float theta = 2.0f * 3.141592f * (float)(i) / 10.f;  // 角度を計算
		float x = 1.f * cos(theta);     // x座標
		float z = 1.f * sin(theta);

		light.obj->SetPos(Vector3(x, 1, z));

		light.obj->SetRot(Vector3(Radian(180),0,0));

		light.swayValue = {
			RRandom::RandF(0.5f, 1.f) * 45.f * (-1.f * (float)RRandom::Rand(0,1)),
			RRandom::RandF(0.5f, 1.f) * 45.f * (-1.f * (float)RRandom::Rand(0,1)),
			RRandom::RandF(0.5f, 1.f) * 45.f * (-1.f * (float)RRandom::Rand(0,1)),
		};
		light.swayTimer.SetLimitTime(RRandom::Rand(120, 200));
	}
	easingTimer_.SetLimitTime(80);
}

void FeverLight::Update()
{
	
	if (isFever_)
	{
		rot_.y += Radian(1.5f);
		easingTimer_.AddTime();
		

		pos_.y = Easing::Bounce::easeOut(
			easingTimer_.GetTimeRate(),
			defaultPos_.y,
			feverPos_.y - defaultPos_.y,
			1.f);
	}
	else {
		if(easingTimer_.GetTimer() > 0)easingTimer_.AddTime(-1);

		pos_.y = 
			Easing::Back::easeOut(
			defaultPos_.y,
			feverPos_.y,
			easingTimer_.GetTimeRate());
	}
	

	flower_->SetPos(pos_);
	flower_->SetRot(rot_);

	flower_->Update();
	for (int32_t i = 0; i < lights_.size(); i++) {
		Vector3 rot = Vector3(Radian(180), 0, 0);
		//ライトを揺らす処理
		if (isFever_) {
			float oneRoundTime = (float)lights_[i].swayTimer.GetLimitTimer();
			float timer = (float)lights_[i].swayTimer.GetTimer();
			rot += {
				Radian(UpAndDown(oneRoundTime, lights_[i].swayValue.x, timer)),
				Radian(UpAndDown(oneRoundTime, lights_[i].swayValue.y, timer)),
				Radian(UpAndDown(oneRoundTime, lights_[i].swayValue.z, timer)),
			};
			lights_[i].obj->SetRot(rot);
			lights_[i].swayTimer.AddTime();
			if (lights_[i].swayTimer.GetIsEnd())lights_[i].swayTimer.Reset();

			//スケールを大きくする
			MoveTo(lightDefaultScale_, 0.1f, lightScale_);
		}
		else {
			//スケールを小さくする
			MoveTo(Vector3(0,0,0), 0.01f, lightScale_);
		}
		lights_[i].obj->SetScale(lightScale_);
		lights_[i].obj->Update();
	}
}

void FeverLight::Draw()
{
	PipelineManager::PreDraw("Object3D_NotCull", TRIANGLELIST);
	flower_->Draw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	for (int32_t i = 0; i < lights_.size(); i++) {
		lights_[i].obj->Draw();
	}
}
