#pragma once
#include "IPostEffect.h"
#include "MultiTexture.h"
#include <array>

class HighLumi :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
};

class GaussianBlur :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
};

class Bloom {
private:
	std::unique_ptr<HighLumi> highLumi_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	std::unique_ptr<MultiTexture> compo_;
public:
	Bloom();
	void Update();
	void PreDraw();
	void Draw();
};

