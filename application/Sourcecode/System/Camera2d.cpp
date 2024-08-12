#include "Camera2d.h"
#include <imgui.h>
#include "RRandom.h"
#include "mInput.h"
#include "HitStop.h"

Camera2d::Camera2d()
{
	scrollLeftX_ = 500;
	scrollRightX_ = 700;
	scrollUpY_ = 300;
	scrollDownY_ = 550;
	scrollValue_ = {0,0};
	speed_ = 0.2f;
	debugSpeed_ = 10.f;

	leftSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	leftSprite_.SetPos(Vector2(scrollLeftX_,400));
	leftSprite_.SetScale(Vector2(7, 800));
	rightSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	rightSprite_.SetPos(Vector2(scrollRightX_, 400));
	rightSprite_.SetScale(Vector2(7, 800));

	upSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	upSprite_.SetPos(Vector2(400, scrollUpY_));
	upSprite_.SetScale(Vector2(1500, 7));
	downSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	downSprite_.SetPos(Vector2(400, scrollDownY_));
	downSprite_.SetScale(Vector2(1500, 7));

	isDebug_ = false;
	isColDisplay_ = false;

	respawnTimer_.SetLimitTime(90);
	respawnTimer_.SetTime(respawnTimer_.GetLimitTimer());
}

void Camera2d::Init()
{
	scrollValue_ = { 0,0 };
	speed_ = 0.2f;
}

Camera2d* Camera2d::GetInstance()
{
	static Camera2d instance;
	return &instance;
}

void Camera2d::Update()
{
#ifdef _DEBUG
	if (Key::TriggerKey(DIK_Q)) isDebug_ = isDebug_ ? false : true;
#endif // _DEBUG
	if (isDebug_ == false)
	{
		leftSprite_.SetColor(Color(255, 255, 255, 100));
		rightSprite_.SetColor(Color(255, 255, 255, 100));
		upSprite_.SetColor(Color(255, 255, 255, 100));
		downSprite_.SetColor(Color(255, 255, 255, 100));
		//左にスクロール
		if (scrollLeftX_ > playerPos_.x)
		{
			float distance = scrollLeftX_ - playerPos_.x;
			scrollValue_.x -= distance * speed_;
		}
		//右にスクロール
		if (scrollRightX_ < playerPos_.x)
		{
			float distance = scrollRightX_ - playerPos_.x;
			scrollValue_.x -= distance * speed_;
		}

		//上にスクロール
		if (scrollUpY_ > playerPos_.y)
		{
			float distance = scrollUpY_ - playerPos_.y;
			scrollValue_.y -= distance * speed_;
		}
		//下にスクロール
		if (scrollDownY_ < playerPos_.y)
		{
			float distance = scrollDownY_ - playerPos_.y;
			scrollValue_.y -= distance * speed_;
		}
	}
	else
	{
		DebugUpdate();
	}

	if (isColDisplay_) {
		leftSprite_.Update();
		rightSprite_.Update();
		upSprite_.Update();
		downSprite_.Update();
	}
	//カメラシェイク
	if (isCameraShake_) {
		CameraShakeUpdate();
	}
	//リスポーンのタイマーが終了したらスクロール開始位置を元に戻す
	respawnTimer_.AddTime();
	if (respawnTimer_.GetIsEnd()) {
		scrollLeftX_ = 500;
		scrollRightX_ = 700;
		scrollUpY_ = 300;
		scrollDownY_ = 550;
	}
}

void Camera2d::DrawCol()
{
	if (isColDisplay_) {
		leftSprite_.Draw();
		rightSprite_.Draw();
		upSprite_.Draw();
		downSprite_.Draw();
	}
}

void Camera2d::DrawImGui()
{
	ImGui::Begin("camera");
	float* scrollValue[2] = { &scrollValue_.x, &scrollValue_.y };
	ImGui::InputFloat2("scrollValue", *scrollValue);
	ImGui::SliderFloat("zoomRate", &zoomRate_, 0.f, 1.f);
	std::string status = "Now Status : ";
	if (isDebug_)status += "Debug";
	else status += "Normal";
	ImGui::Text(status.c_str());

	ImGui::Text("操作方法 ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Qキー : デバッグ オン／オフ切り替え");
		ImGui::TextUnformatted("arrow : カメラ移動");
		ImGui::TextUnformatted("LCTRL + arrow UP or Down : カメラズーム");
		ImGui::TextUnformatted("LSHIFT長押し : カメラスピードUP");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::Checkbox("スクロール開始位置の描画",&isColDisplay_);

	ImGui::End();
}

void Camera2d::StartShake(int32_t time, float power)
{
	isCameraShake_ = true;
	power_ = power;
	shakeTimer_.SetLimitTime(time);
	shakeTimer_.Reset();
}

void Camera2d::Reset(const Vector2& playerPos)
{
	scrollValue_.x = playerPos.x - scrollLeftX_;
	scrollValue_.y = playerPos.y - scrollDownY_;
}

void Camera2d::CameraShakeUpdate()
{
	float power = power_ * (1.f - shakeTimer_.GetTimeRate());
	shakeValue_ = {
		RRandom::RandF(-power,power),
		RRandom::RandF(-power,power),
	};
	shakeTimer_.AddTime();
	if (shakeTimer_.GetIsEnd()) {
		isCameraShake_ = false;
		shakeValue_ = { 0,0 };
	}
}

void Camera2d::DebugUpdate()
{
	float speedRate = 1.f;
	if (Key::PushKey(DIK_LSHIFT)) speedRate = 2.f;
	if (Key::PushKey(DIK_LEFT)) scrollValue_.x -= debugSpeed_* speedRate;
	if (Key::PushKey(DIK_RIGHT)) scrollValue_.x += debugSpeed_* speedRate;
	if (Key::PushKey(DIK_UP)) scrollValue_.y -= debugSpeed_	* speedRate;
	if (Key::PushKey(DIK_DOWN)) scrollValue_.y += debugSpeed_* speedRate;

	if (Key::PushKey(DIK_LCONTROL) && Key::TriggerKey(DIK_UP)) zoomRate_ += 0.1f;
	if (Key::PushKey(DIK_LCONTROL) && Key::TriggerKey(DIK_DOWN)) zoomRate_ -= 0.1f;
	
}

