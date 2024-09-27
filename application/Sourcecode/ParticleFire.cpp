#include "ParticleFire.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

/**
 * @file ParticleHitAttack.cpp
 * @brief 火のパーティクルクラス
 */

#pragma region 炎エフェクト
ParticleFire::ParticleFire() :
	IParticle("Fire"),
	vertexCount(192)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Particle");
	billBoard = BillBoard::None;
	state_ = PipeLineState::Add;
}

void ParticleFire::Add()
{
	////////////////////////////////////////////////
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
		fireParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = fireParticles_.back();

		Vector3 vec = {
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1)
		};
		Vector3 addrot = {
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1),
			RRandom::RandF(-1,1)
		};
		/////////////////////////////////////////////
		//めんどくさいならここはマジックナンバーとかでもOK
		p.position = emitter_->pos + vec;
		p.end_frame = emitter_->time;
		p.scale = emitter_->scale;
		p.color = { 255,100,0,255 };

		baseP = p;
	}
}

void ParticleFire::MoveUpdate()
{
	///////////////////////////
	//パーティクルを削除する処理
	for (int32_t i = 0; i < fireParticles_.size(); i++)
	{
		fireParticles_[i].rate = (float)fireParticles_[i].frame / (float)fireParticles_[i].end_frame;

		if (fireParticles_[i].frame >= fireParticles_[i].end_frame)
		{
			fireParticles_.erase(fireParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}
	//////////////////////////
	
	//ここにパーティクルの動きの処理を書く
	uint32_t index = 0;
	for (auto& p : fireParticles_)
	{
		p.frame++;

		float f = (float)p.frame / p.end_frame;

		p.position += { 0, 0.2f, 0 };

		if (f < 0.5f)p.color.a = f * 355.f;
		else p.color.a = (1 - f) * 355.f;

		p.scale -= 0.01f;
		p.scale = Max(0.f, p.scale);

		//最終的に（std::vector<Particle> particles_;）に代入する
		particles_[index] = p;
		index++;
	}
}
#pragma endregion
