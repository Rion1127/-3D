#include "Default.hlsli"

VSOutput main(float4 pos : POSITION)
{
    VSOutput output; // �W�I���g���V�F�[�_�[�ɓn���l
    
    output.pos = pos;
    
    return output;
}