#pragma once
#include <memory>
#include "EmptyScene.h"
#include "mInput.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Util.h"
class SceneManager
{
public:
	static void Ini();
	static void Update();
	static void Draw();

	template <class NextScene>
	//なぜかインラインで宣言しないとエラーが起こる
	static inline void Transition() {
		/*deletee*/ currentScene_.release();
		currentScene_ = std::unique_ptr<NextScene>(new NextScene());
		currentScene_->Ini();
	}
	static void SetChangeStart(uint32_t sceneNum) {
		if (isNext_ == false) {
			
			sceneNum_ = sceneNum;
			isNext_ = true;
			isDifferent = true;
		}
	}

	static std::unique_ptr<EmptyScene> currentScene_;

	
	static uint32_t sceneNum_;
	static bool isNext_;
	static bool isDifferent;
};



