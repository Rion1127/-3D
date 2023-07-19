#include "TextureBlend.h"
#include "Util.h"
#include "PipelineManager.h"

TextureBlend::TextureBlend()
{
	textureblend_ = std::move(std::make_unique<MultiTexture>(0));
	obj_ = MakeUniquePtr<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("cube"));
	obj_->GetTransform()->SetPosition({ 3,0,0 });

	textures_.mainTex = *TextureManager::GetInstance()->GetTexture("Grass");
	textures_.subTex = *TextureManager::GetInstance()->GetTexture("Dirt");
	textures_.mascTex = *TextureManager::GetInstance()->GetTexture("FirldMask");

	textureblend_->AddTexture(&textures_.mainTex);
	textureblend_->AddTexture(&textures_.subTex);
	textureblend_->AddTexture(&textures_.mascTex);
	textureblend_->Create();

}

void TextureBlend::Draw()
{
	obj_->Update();
	
	textureblend_->SetGraphicsRootDescriptorTable("TextureBlend",4);
	obj_->DrawVert();
}