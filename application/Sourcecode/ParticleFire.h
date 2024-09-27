#pragma once
#include "IParticle.h"

/**
 * @file ParticleHitAttack.h
 * @brief 火のパーティクルクラス
 */

class ParticleFire final :
	public IParticle
{
public:
	ParticleFire();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<IParticle::Particle> fireParticles_;
};
