#include "Noise.h"
#include "Util.h"
#include "RRandom.h"
Noise::Noise()
{
	constBuff_ = CreateBuff(constMap_);
}

void Noise::TransferBuff()
{
	time_++;
	
	//定数バッファのマッピング
	ConstBuffTime* map{};
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	map->time = time_;
	map->power = RRandom::RandF(0.3f, 0.5f);
	map->width = RRandom::RandF(0.01f, 0.1f);

	constBuff_->Unmap(0, nullptr);
}

void Noise::SendToShader()
{
	SetBuff(1, constBuff_.Get());
}