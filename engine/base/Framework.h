#pragma once
#include "Framework.h"
#include "mInput.h"
#include "Texture.h"
#include "Util.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "GameScene.h"
#include "mSound.h"
#include "SceneManager.h"
#include "PipelineManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "LoadManager.h"
#include "MultipleRenderTarget.h"
#include "MultiTexture.h"
#include "Bloom.h"
#include "Noise.h"
#include "GaussianBlur.h"

enum class PostEffectName {
	None,
	Gaussian,
	Bloom,
	Noise,
	END
};

class Framework
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	LoadManager loadManager_;
	
	std::unique_ptr<Bloom> bloom_;
	std::unique_ptr<Noise> noise_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	size_t postEffectnum = 0;
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//実行
	void Run();
};

