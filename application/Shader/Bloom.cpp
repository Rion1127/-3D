#include "Bloom.h"
#include "SceneManager.h"
#include "PipelineManager.h"

Bloom::Bloom()
{
	highLumi_= std::move(std::make_unique<PostEffect>());
	gaussianBlur_ = std::move(std::make_unique<PostEffect>());
	compo_ = std::move(std::make_unique<PostEffect>());
}

void Bloom::Update() {
	highLumi_->PUpdate();
	gaussianBlur_->PUpdate();
	compo_->PUpdate();
}

void Bloom::PreDraw()
{
	highLumi_->PreDrawScene();
	SceneManager::Draw();
	highLumi_->PostDrawScene();

	gaussianBlur_->PreDrawScene();
	highLumi_->Draw("HighLumi");
	gaussianBlur_->PostDrawScene();

	compo_->PreDrawScene();
	gaussianBlur_->Draw("Bloom");
	compo_->PostDrawScene();
}

void Bloom::Draw()
{
	compo_->Draw("Bloom");
}