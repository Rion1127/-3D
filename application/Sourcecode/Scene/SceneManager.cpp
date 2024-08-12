#include "SceneManager.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"

#include "GameScene.h"
#include "ClearScene.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "GameOverScene.h"
#include "Framework.h"
#include "imgui.h"
#include "ClearScene.h"
#include <filesystem>
#include "Util.h"

/**
 * @file SceneManager.cpp
 * @brief シーンを管理するクラス
 */

std::unique_ptr<IScene> SceneManager::scurrentScene_ = nullptr;
SceneName SceneManager::ssceneName_;
bool SceneManager::sisSetNext_ = false;
Timer SceneManager::animeTimer_;
Sprite SceneManager::whiteSprite_;
std::vector<std::string> SceneManager::sceneNames_;
std::vector<std::string> SceneManager::stageNames_;
bool SceneManager::isSceneChange_ = false;
std::string SceneManager::sceneName_ = "Title";
std::string SceneManager::stageName_ = "SceneSelect";

void SceneManager::Ini()
{
	sceneNames_.emplace_back("Title");
	sceneNames_.emplace_back("Game");
	sceneNames_.emplace_back("ClearScene");
	
	StageListUpdate();
	//ビルド開始時のシーン
	Transition<GameScene>();
	//Transition<TitleScene>();
	//Transition<ClearScene>();
	//Transition<StageEditorScene>();

	animeTimer_.SetLimitTime(100);
	whiteSprite_.Init();
	whiteSprite_.SetColor(Color(220, 220, 220, 0));
	whiteSprite_.SetAnchor({ 0.f,0.f });
	whiteSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
}

void SceneManager::Update()
{
	scurrentScene_->Update();

#ifdef _DEBUG
	
	if (Framework::isImguiDisplay_) {
	
	}

	if (isSceneChange_)
	{
		if (sceneName_ == "Title") {
			stageName_ = "SceneSelect";
			Transition<TitleScene>();
		}
		else if (sceneName_ == "Game")Transition<GameScene>();
		else if (sceneName_ == "ClearScene")Transition<ClearScene>();
		isSceneChange_ = false;
	}

#endif // _DEBUG

	//切り替えアニメーション開始
	if (sisSetNext_)
	{
		animeTimer_.AddTime(1);
		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();
		//シーンチェンジアニメーションが終了したら切り替える
		if (animeTimer_.GetIsEnd())
		{
			SceneChange();
		}
	}
	else
	{
		if (animeTimer_.GetTimer() > 0)
		{
			animeTimer_.AddTime(-5);
		}

		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();
	}
}

void SceneManager::Draw()
{
	//ゲーム描画
	scurrentScene_->Draw();

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	whiteSprite_.Draw();

#ifdef _DEBUG
	if (Framework::isImguiDisplay_) {
		ImGui::Begin("SceneManager");
		//プルダウンメニューでイージングタイプを選択できるようにする
		if (ImGui::BeginCombo("SceneNema", sceneName_.c_str()))
		{
			for (uint32_t i = 0; i < (uint32_t)sceneNames_.size(); ++i)
			{
				//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
				const bool is_selected = (sceneName_ == sceneNames_[i]);
				if (ImGui::Selectable(sceneNames_[i].c_str(), is_selected))
				{
					sceneName_ = sceneNames_[i].c_str();
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		
		if (ImGui::Button("Change Scene", ImVec2(50, 50))) {
			isSceneChange_ = true;
		}
		ImGui::End();
	}
#endif // _DEBUG
}

void SceneManager::DrawPostEffect()
{
	scurrentScene_->DrawPostEffect();
}

void SceneManager::DrawRenderTexture()
{
	scurrentScene_->DrawRenderTexture();
}

void SceneManager::SceneChange()
{
	if (ssceneName_ == SceneName::Title)
	{
		stageName_ = "SceneSelect";
		Transition<TitleScene>();
	}
	else if (ssceneName_ == SceneName::Game)
	{
		Transition<GameScene>();
	}
	else if (ssceneName_ == SceneName::GameClear) {
		Transition<ClearScene>();
	}
	else if (ssceneName_ == SceneName::GameOver)
	{
		Transition<GameOverScene>();
	}
	
	sisSetNext_ = false;
}

void SceneManager::SetChangeStart(const SceneName sceneName) {
	if (sisSetNext_ == false)
	{
		whiteSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
		ssceneName_ = sceneName;
		sisSetNext_ = true;
		//ゲームシーンへ
		if (sceneName == SceneName::Title)
		{
			animeTimer_.SetLimitTime(100);
		}
		//ゲームオーバーシーンへ
		else if (sceneName == SceneName::Game)
		{
			animeTimer_.SetLimitTime(100);
		}
		//ゲームオーバーシーンへ
		else if (sceneName == SceneName::GameOver)
		{
			animeTimer_.SetLimitTime(120);
		}
		//ゲームクリアシーン
		else if (sceneName == SceneName::GameClear)
		{
			animeTimer_.SetLimitTime(120);
		}

	}
}

void SceneManager::StageListUpdate()
{
	//現在のファイルディレクトリを取得
	fs::path p = fs::current_path();
	std::string dir = p.generic_string();
	size_t strPos = dir.find("Json");
	//カレントディレクトリが変化していたら何も代入しない
	if (strPos == std::string::npos){
		dir = "application/Resources/Json/";
	}
	else {
		dir = "";
	}

	stageNames_ = FindFileNames(dir, ".json", false);
}
