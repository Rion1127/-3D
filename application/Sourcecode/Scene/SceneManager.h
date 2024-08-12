#pragma once
#include <memory>
#include "IScene.h"
#include "mInput.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Util.h"
#include "Timer.h"


/**
 * @file SceneManager.cpp
 * @brief シーンを管理するクラス
 */

enum class SceneName {
	Title,
	StageSelect,
	Game,
	GameOver,
	GameClear,
	Debug,
	None
};

class SceneManager
{
private:
	static std::unique_ptr<IScene> scurrentScene_;

	static SceneName ssceneName_;
	static bool sisSetNext_;
	static Timer animeTimer_;
	static Sprite whiteSprite_;
	static std::vector<std::string> sceneNames_;
	static std::vector<std::string> stageNames_;
	static bool isSceneChange_;
	static std::string sceneName_;	//シーンの文字列
	static std::string stageName_;	//ステージの文字列
public:
	static void Ini();
	static void Update();
	static void Draw();
	static void DrawPostEffect();
	static void DrawRenderTexture();

	template <class NextScene>
	static inline void Transition() {
		scurrentScene_.reset();
		scurrentScene_ = std::move(std::make_unique<NextScene>());
		scurrentScene_->Init();
	}
	static void SetChangeStart(const SceneName sceneName);
	static void SetStageName(const std::string stageName) { stageName_ = stageName; };
	static void StageListUpdate();
public:
	static bool GetIsSetNext() { return sisSetNext_; }
	static SceneName GetSceneName() { return ssceneName_; }
	static std::string GetStageName() { return stageName_; }
	static std::vector<std::string> GetStageNames() { return stageNames_; }
private:
	static void SceneChange();
};



