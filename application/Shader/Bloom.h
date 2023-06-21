#pragma once
#include "PostEffect.h"
#include "MultiTexture.h"
#include <array>
class Bloom {
private:
	std::unique_ptr<PostEffect> highLumi_;
	std::unique_ptr<PostEffect> gaussianBlur_;
	std::unique_ptr<MultiTexture> compo_;
public:
	Bloom();
	void Update();
	void PreDraw();
	void Draw();
};