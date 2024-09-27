#pragma once
#include "IParticle.h"

/**
 * @file ParticleHitAttack.h
 * @brief 火のパーティクルクラス
 */

class ParticleAscension final :
	public IParticle
{
private:
	struct AscensionParticle : public Particle
	{
		Vector3 basePos;
		Vector3 velocity;
		Vector3 addvelocity;
		Vector3 addRot;
		float baseScale;
		TimerFloat timer_;
	};
public:
	ParticleAscension();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	std::vector<AscensionParticle> ascensionParticles_;

	float endScale_;
	Color color_;
};
