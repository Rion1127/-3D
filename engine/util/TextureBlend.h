#pragma once
#include "MultiTexture.h"
#include "Object3d.h"
#include "Texture.h"
#include <array>
#include "Sprite.h"

struct Textures{
	Texture mainTex;
	Texture subTex;
	Texture mascTex;
};

class TextureBlend
{
private:
	std::unique_ptr<MultiTexture> textureblend_;
	std::unique_ptr<Object3d> obj_;
	Textures textures_;
	Sprite
public:
	TextureBlend();
	void Draw();
};
