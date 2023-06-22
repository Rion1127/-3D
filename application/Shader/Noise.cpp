#include "Noise.h"
#include "Util.h"
Noise::Noise()
{
	constBuffT_ = CreateBuff(constMap_);
}

void Noise::TransferBuff()
{
	time_++;
	//// �萔�o�b�t�@�Ƀf�[�^�]��
	//constMap_->time = time_; // �s��̍���
	
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBuffTime* map{};
	HRESULT result = constBuffT_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	map->time = time_;

	constBuffT_->Unmap(0, nullptr);
}
void Noise::SendToShader()
{
	SetBuff(1, constBuffT_.Get());
}