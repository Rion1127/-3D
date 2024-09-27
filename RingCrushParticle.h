#pragma once
#include "IParticle.h"

/**
 * @file ParticleHitAttack.h
 * @brief 大群の輪っか壊れた時のパーティクルクラス
 */

class RingCrushParticle final :
	public IParticle
{
public:
	RingCrushParticle();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<IParticle::Particle> RingCrushParticles_;
};