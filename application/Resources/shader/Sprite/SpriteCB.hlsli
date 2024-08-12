// ���[���h�s��
cbuffer cbMatWorld : register(b0)
{
    matrix matWorld;
}

// �F
cbuffer cbColor : register(b1)
{
    float4 baseColor;
    float3 texColorRate;
    float alphaClipVal;
}

// �e�N�X�`���ݒ�
cbuffer cbTextureSetting : register(b2)
{
    float2 texSeqSize; // �摜�T�C�Y
    float2 texRange; // �G�͈̔�
    
    float2 texSizeRatio; // �e�N�X�`���T�C�Y�䗦

    float2 texSeqNum; // �A�Ԉʒu
    float2 texSeqMax; // �A�Ԑ�
		
    float texFlipX; // X�����]
    float texFlipY; // Y�����]

    float2 texAnchor; // �A���J�[�|�C���g
    float2 texOffset; // �I�t�Z�b�g
    
    float2 uvTiling; // �^�C�����O
    float2 uvOffset; // �I�t�Z�b�g
}
