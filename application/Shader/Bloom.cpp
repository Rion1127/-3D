#include "Bloom.h"
#include "SceneManager.h"
#include "PipelineManager.h"

Bloom::Bloom()
{
	highLumi_= std::move(std::make_unique<PostEffect>());
	gaussianBlur_ = std::move(std::make_unique<PostEffect>());
	compo_ = std::move(std::make_unique<MultiTexture>());
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

	compo_->PreDrawSceneAssin(0);
	gaussianBlur_->Draw("Gaussian");
	compo_->PostDrawSceneAssin(0);

	compo_->PreDrawSceneAssin(1);
	SceneManager::Draw();
	compo_->PostDrawSceneAssin(1);
}

void Bloom::Draw()
{
	compo_->Draw("Bloom");
}