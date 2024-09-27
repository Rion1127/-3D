#include "ParticleAscension.h"
#include "RRandom.h"
#include "Easing.h"
#include "Util.h"

/**
 * @file ParticleHitAttack.cpp
 * @brief 火のパーティクルクラス
 */

#pragma region 炎エフェクト
ParticleAscension::ParticleAscension() :
	IParticle("Particle"),
	vertexCount(192)
{
	Init(vertexCount);
	texture = *TextureManager::GetInstance()->GetTexture("Star4");
	billBoard = BillBoard::None;
	state_ = PipeLineState::Add;
}

void ParticleAscension::Add()
{
	endScale_ = 3.f;
	//transform_.position_ = emitter_->pos;
	color_ = { 255,175,60,255 };
	for (int i = 0; i < emitter_->addNum; i++) {
		//指定した最大頂点数超えてたら生成しない
		if (particles_.size() >= vertexCount) {
			return;
		}
		//リストに要素を追加
		particles_.emplace_back();
		ascensionParticles_.emplace_back();
		//追加した要素の参照
		auto& baseP = particles_.back();
		auto& p = ascensionParticles_.back();

		Vector3 vec = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			-RRandom::RandF(0,emitter_->addVec.y * 2.f),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		Vector3 addvec = {
			0,
			RRandom::RandF(-emitter_->addVec.y / 10.f,-emitter_->addVec.y / 50.f),
			0
		};
		Vector3 addrot = {
			RRandom::RandF(-emitter_->addVec.x,emitter_->addVec.x),
			RRandom::RandF(-emitter_->addVec.y,emitter_->addVec.y),
			RRandom::RandF(-emitter_->addVec.z,emitter_->addVec.z)
		};
		float scale = RRandom::RandF(emitter_->scale * 4.f, emitter_->scale * 4.f);

		p.basePos = emitter_->pos;
		p.position = emitter_->pos;
		p.end_frame = emitter_->time;
		p.velocity = vec;
		p.addvelocity = addvec;
		p.scale = scale;
		p.baseScale = scale;
		//p.addRot = addrot * 3.f;
		p.color = color_;
		p.timer_.SetLimitTime(RRandom::RandF(5, 10));

		baseP = p;
	}
}

void ParticleAscension::MoveUpdate()
{
	for (int32_t i = 0; i < ascensionParticles_.size(); i++) {
		ascensionParticles_[i].rate = (float)ascensionParticles_[i].frame / (float)ascensionParticles_[i].end_frame;

		if (ascensionParticles_[i].frame >= ascensionParticles_[i].end_frame) {
			ascensionParticles_.erase(ascensionParticles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}

	uint32_t index = 0;
	for (auto& p : ascensionParticles_) {
		p.frame += 1;
		p.timer_.AddTime(1.f);

		float f = (float)p.frame / p.end_frame;

		p.velocity += p.addvelocity;

		p.position += p.velocity;
		p.rot += p.addRot;

		p.scale = Easing::Quint::easeIn(p.baseScale, 0.f, f);

		MoveTo({ 0,0,0 }, 0.003f , p.addRot);


		if (p.timer_.GetIsEnd()) {
			p.timer_.Reset();

			Color color = color_ * 2.f;

			p.color = (p.color.a == color_.a) ? color : color_;
		}

		p.color.a = 255.f - 255.f * p.timer_.GetTimeRate();

		particles_[index] = p;

		index++;
	}
}
#pragma endregion
