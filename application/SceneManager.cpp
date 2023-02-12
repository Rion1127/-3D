#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"


void SceneManager::Ini()
{
	//ビルド開始時のシーン
	Transition<ParticleScene>();
	sceneChangeSprite.Ini();
	sceneChangePos = {
		(float)-(WinAPI::GetInstance()->win_width / 2) ,
		(float)(WinAPI::GetInstance()->win_height / 2) ,
	};
	sceneChangeSprite.SetPos(sceneChangePos);

	sceneChangeGraph = TextureManager::GetInstance()->LoadGraph("white1×1.png");
}

void SceneManager::Update()
{
	currentScene->Update();

//	//シーンが切り替わるとき
//	if (currentScene->GetIsNext() == 1) {
//#pragma region シーン切り替え
//		float changeTime_ = 0.5f;
//		// 現在の時間
//		int nowCount = GetNowCount();
//
//		// 経過時間[s]
//		float nowTime = (float)((nowCount - changeStartCount_) / 1000.0f);
//
//		// 経過時間の割合
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeOut(
//				(float)-(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//#pragma endregion
//		//イージングが終わったらシーンを切り替える
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//			//コンティニューするシーンをタイトルとゲームオーバー以外のとき代入する
//			if (sceneNum_ != SceneNum::TITLE_ &&
//				sceneNum_ != SceneNum::GAMEOVER_) {
//				continueScene = sceneNum_;
//			}
//			Transition<GameScene>();
//		}
//	}
//	//シーンが変わったら
//	else if (currentScene->GetIsNext() == 2) {
//#pragma region シーン切り替え
//		float changeTime_ = 0.5f;
//		// 現在の時間
//		int nowCount = GetNowCount();
//
//
//		if (isDifferent == true) {
//			different = nowCount - changeStartCount_;
//			isDifferent = false;
//		}
//		// 経過時間[s](590は初期化で遅れる時間分引いているパワーコード後で修正)
//		float nowTime = (float)((nowCount - changeStartCount_ - different) / 1000.0f);
//
//		// 経過時間の割合
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeIn(
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width * 1.5f),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//#pragma endregion
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//			currentScene->SetIsNext(0);
//		}
//	}
//	//ゲームオーバーの時
//	else if (currentScene->GetIsNext() == 3) {
//		float changeTime_ = 0.5f;
//
//		// 現在の時間
//		int nowCount = GetNowCount();
//
//		// 経過時間[s]
//		float nowTime = (float)((nowCount - changeStartCount_) / 1000.0f);
//
//		// 経過時間の割合
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeOut(
//				(float)-(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//		//イージングが終わったらシーンを切り替える
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//
//
//			Transition<GameScene>();
//
//		}
//	}
}

void SceneManager::Draw()
{

	//ゲーム描画
	currentScene->Draw();

	//シーン切り替え
	sceneChangeSprite.PreDraw();
	if (currentScene->GetIsNext()) {
		sceneChangeSprite.Draw(sceneChangeGraph);
	}
}

std::unique_ptr<EmptyScene> SceneManager::currentScene = nullptr;

Sprite SceneManager::sceneChangeSprite;
Vector2  SceneManager::sceneChangePos;
uint32_t SceneManager::sceneChangeGraph;
int SceneManager::changeStartCount_;
int SceneManager::sceneNum_;
bool SceneManager::isNext_;
int SceneManager::continueScene = SceneNum::STAGE1_;
bool SceneManager::isDifferent = true;
int SceneManager::different = 0;