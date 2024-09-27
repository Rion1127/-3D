#include "RingCrushParticle.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

/**
 * @file ParticleHitAttack.cpp
 * @brief 大群の輪っか壊れた時のパーティクルクラス
 */

RingCrushParticle::RingCrushParticle() :
	IParticle("Particle"),
	vertexCount(48)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Particle");
	billBoard = BillBoard::None;
	state_ = PipeLineState::Alpha;
}

void RingCrushParticle::Add()
{
	//パーティクルの生成・初期化
	for (int i = 0; i < emitter_->addNum; i++)
	{
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount)
		{
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		RingCrushParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = RingCrushParticles_.back();

		float pX = emitter_->pos.x + 1.f * cosf(Radian((360.f / 48) * (float)i));
		float pY = emitter_->pos.y;
		float pZ = emitter_->pos.z + 1.f * sinf(Radian((360.f / 48) * (float)i));

		Vector3 vec = {
			pX * 1.f,
			RRandom::RandF(-0.05f,0.05f),
			pZ * 1.f
		};
		Vector3 addrot = {
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1)
		};

		p.position = Vector3(pX,pY,pZ);
		p.end_frame = emitter_->time;
		p.scale = 2.f;
		p.color = { 255,100,0,255 };

		baseP = p;
	}
}

void RingCrushParticle::MoveUpdate()
{
	//パーティクルを削除する処理
	for (int32_t i = 0; i < RingCrushParticles_.size(); i++)
	{
		RingCrushParticles_[i].rate = (float)RingCrushParticles_[i].frame / (float)RingCrushParticles_[i].end_frame;

		if (RingCrushParticles_[i].frame >= RingCrushParticles_[i].end_frame)
		{
			RingCrushParticles_.erase(RingCrushParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	//ここにパーティクルの動きの処理を書く
	uint32_t index = 0;
	for (auto& p : RingCrushParticles_)
	{
		p.frame++;

		//float f = (float)p.frame / p.end_frame;

		float pX = emitter_->pos.x + 1.f * cosf(Radian((360.f / 48.f) * (float)index));
		//float pY = emitter_->pos.y;
		float pZ = emitter_->pos.z + 1.f * sinf(Radian((360.f / 48.f) * (float)index));

		Vector3 vec = {
			pX * 1.f,
			RRandom::RandF(-0.05f,0.05f),
			pZ * 1.f
		};

		p.position += vec;

		p.color = Color::kLightblue;

		p.scale -= 0.01f;
		p.scale = Max(0.f, p.scale);

		//最終的に（std::vector<Particle> particles_;）に代入する
		particles_[index] = p;
		index++;
	}
}
