#include "ParticleManager.h"
#include "PipelineManager.h"

#include "ParticleFire.h"

/**
 * @file ParticleManager.cpp
 * @brief パーティクルを管理するクラス
 */

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::AllReset()
{
	if (emitters_.size() <= 0)return;
	std::list<std::shared_ptr<IEmitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();) {
		(*itr)->isActive = false;
		itr++;
	}
}

ParticleManager::ParticleManager()
{
}

void ParticleManager::InitParticle(const std::string& name)
{
	auto& emitter = emitters_.back();
	if (name == "fireBall") {
		emitter->particle = std::make_unique<ParticleFire>();
		//各々のパーティクルのAdd()の中にマジックナンバーで書いてもいい
		emitter->addVec = { 0.2f,0.2f, 0.2f, };
		emitter->addNum = 5;
		emitter->isActive = true;
		emitter->time = 20;
		emitter->scale = 1.f;
	}
}

void ParticleManager::Update()
{
	std::list<std::shared_ptr<IEmitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();) {

		//パーティクルの数が0になったら
		if ((*itr)->particle->GetParticleNum() == 0) {
			itr = emitters_.erase(itr);
			continue;
		}
		if ((*itr)->isActive) {
			//座標を更新
			(*itr)->popCoolTime_.AddTime(1);
			if ((*itr)->popCoolTime_.GetIsEnd()) {
				(*itr)->particle->Add();

				(*itr)->popCoolTime_.Reset();
			}
		}
		(*itr)->particle->Update();

		(*itr)->particle->TransferBuff();
		itr++;
	}

}

void ParticleManager::Draw()
{
	std::string shaderName = "Particle";
	for (auto& emitter : emitters_) {
		PipelineManager::PreDraw(emitter->particle->GetShaderName(), POINTLIST, emitter->particle->GetPipelineState());
		emitter->particle->Draw();
	}
}

void ParticleManager::AddParticle(const std::string& name, const std::shared_ptr<IEmitter>& emitter)
{
	emitters_.emplace_back();
	emitters_.back() = emitter;
	InitParticle(name);
	emitters_.back()->particle->SetEmitter(emitter.get());
	emitters_.back()->particle->Add();
}
