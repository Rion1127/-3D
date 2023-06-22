#include "Noise.h"
#include "Util.h"
Noise::Noise()
{
	constBuff_ = CreateBuff(constMap_);
}

void Noise::TransferBuff()
{
	time_++;
	// 定数バッファにデータ転送
	constMap_->time = time_; // 行列の合成
	
}
void Noise::SendToShader()
{
	SetBuff(1, constBuff_.Get());
}