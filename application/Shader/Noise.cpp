#include "Noise.h"
#include "Util.h"
Noise::Noise()
{
	constBuff_ = CreateBuff(constMap_);
}

void Noise::TransferBuff()
{
	time_++;
	// �萔�o�b�t�@�Ƀf�[�^�]��
	constMap_->time = time_; // �s��̍���
	
}
void Noise::SendToShader()
{
	SetBuff(1, constBuff_.Get());
}