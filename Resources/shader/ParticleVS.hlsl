#include "Particle.hlsli"


VSOutput main(float4 pos : POSITION)
{
	//�s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output;
    output.pos = pos;
    return output;
}