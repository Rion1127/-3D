#include "Noise.h"
#include "Util.h"
Noise::Noise()
{
	//constBuff_ = CreateBuff<ConstBuffTime>;
}

void Noise::TransferBuff() {
	//ConstBuffTime* constMap = nullptr;
	//HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	//if (SUCCEEDED(result))
	//{
	//	// �萔�o�b�t�@�Ƀf�[�^�]��
	//	constMap->time = time_; // �s��̍���
	//}
	//constBuff_->Unmap(0,nullptr);
}
void Noise::SendToShader() {
	SetBuff(1, constBuff_.Get());
}