#include "ParticleManager.h"
#include "PipelineManager.h"


ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

ParticleManager::ParticleManager()
{

}

void ParticleManager::Update()
{
	for (auto& particle : particles_)
	{
		particle.second->Update();
	}
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_)
	{
		particle.second->Draw();
	}
}

void ParticleManager::AddParticle(std::string particleName, int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	particles_.find(particleName)->second->Add(addNum, time, pos, addVec, scale);
}
