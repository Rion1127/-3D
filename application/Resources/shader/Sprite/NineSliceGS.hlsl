#include "Default.hlsli"
#include "NineSliceCB.hlsli"

static const uint vertexNum = 4;
static const uint sectionNum = 9;

[maxvertexcount(vertexNum * sectionNum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
    // �e�N�X�`���T�C�Y�����ɂȂ�悤��UV�{�[�_�[��␳����
    // UV : �␳��UV = 1 : ���� (�e�N�X�`���͈� / �X�P�[��)
    float2 adjustedUVBorder = uvBorder * (texRange / scale);
    
    // ---------- �ʒu ---------- //
    
    // �ʒu����
    float2 posLT = texRange * -texAnchor * float2(texFlipX, texFlipY);
    
    // �ʒu�͈�
    float2 posRange = texRange * float2(texFlipX, texFlipY);
    
    // ��؂�̋��E���̈ʒu
    // (�n�[ | 1 | 2 | �I�[)
    float2 borderPos0 = float2(0, 0);
    float2 borderPos1 = posRange * adjustedUVBorder;
    float2 borderPos2 = posRange * (1.0f - adjustedUVBorder);
    float2 borderPos3 = posRange;
    
	// --------- UV���W --------- //
    
    // uv���� = (�ꖇ�̑傫�� * ���Ԗ�) + �󔒕� / �S�̂̑傫��
    float2 uvLT = (texSeqSize * texSeqNum) + ((texSeqSize - texRange) * texAnchor) / (texSeqSize * texSeqMax);
    
    // uv�͈�
    float2 uvRange = (texRange / texSeqSize);
    
    // ��؂�̋��E����UV
    // (�n�[ | 1 | 2 | �I�[)
    float2 borderUV0 = float2(0, 0);
    float2 borderUV1 = uvRange * uvBorder;
    float2 borderUV2 = uvRange * (1.0f - uvBorder);
    float2 borderUV3 = uvRange;
    
    // --------- �z�� --------- //
    
	// �I�t�Z�b�g�l
    float2 offsets[sectionNum][vertexNum] =
    {
        // --------- 00 - 02 --------- //
        { 
            float2(borderPos0.x, borderPos0.y), float2(borderPos1.x, borderPos0.y), 
            float2(borderPos0.x, borderPos1.y), float2(borderPos1.x, borderPos1.y),
        },
        { 
            float2(borderPos1.x, borderPos0.y), float2(borderPos2.x, borderPos0.y), 
            float2(borderPos1.x, borderPos1.y), float2(borderPos2.x, borderPos1.y),
        },
        { 
            float2(borderPos2.x, borderPos0.y), float2(borderPos3.x, borderPos0.y), 
            float2(borderPos2.x, borderPos1.y), float2(borderPos3.x, borderPos1.y),
        },
        
        // --------- 10 - 12 --------- //
        { 
            float2(borderPos0.x, borderPos1.y), float2(borderPos1.x, borderPos1.y), 
            float2(borderPos0.x, borderPos2.y), float2(borderPos1.x, borderPos2.y),
        },
        { 
            float2(borderPos1.x, borderPos1.y), float2(borderPos2.x, borderPos1.y), 
            float2(borderPos1.x, borderPos2.y), float2(borderPos2.x, borderPos2.y),
        },
        { 
            float2(borderPos2.x, borderPos1.y), float2(borderPos3.x, borderPos1.y), 
            float2(borderPos2.x, borderPos2.y), float2(borderPos3.x, borderPos2.y),
        },
        
        // --------- 20 - 22 --------- //
        { 
            float2(borderPos0.x, borderPos2.y), float2(borderPos1.x, borderPos2.y), 
            float2(borderPos0.x, borderPos3.y), float2(borderPos1.x, borderPos3.y),
        },
        { 
            float2(borderPos1.x, borderPos2.y), float2(borderPos2.x, borderPos2.y), 
            float2(borderPos1.x, borderPos3.y), float2(borderPos2.x, borderPos3.y),
        },
        { 
            float2(borderPos2.x, borderPos2.y), float2(borderPos3.x, borderPos2.y), 
            float2(borderPos2.x, borderPos3.y), float2(borderPos3.x, borderPos3.y),
        },
    };
    
    // UV�l
    float2 uvs[sectionNum][vertexNum] =
    {
        // --------- 00 - 02 --------- //
        {
            float2(borderUV0.x, borderUV0.y), float2(borderUV1.x, borderUV0.y),
		    float2(borderUV0.x, borderUV1.y), float2(borderUV1.x, borderUV1.y),
        },
        {
            float2(borderUV1.x, borderUV0.y), float2(borderUV2.x, borderUV0.y),
		    float2(borderUV1.x, borderUV1.y), float2(borderUV2.x, borderUV1.y),
        },
        {
            float2(borderUV2.x, borderUV0.y), float2(borderUV3.x, borderUV0.y),
		    float2(borderUV2.x, borderUV1.y), float2(borderUV3.x, borderUV1.y),
        },
        
        // --------- 10 - 12 --------- //
        {
            float2(borderUV0.x, borderUV1.y), float2(borderUV1.x, borderUV1.y),
		    float2(borderUV0.x, borderUV2.y), float2(borderUV1.x, borderUV2.y),
        },
        {
            float2(borderUV1.x, borderUV1.y), float2(borderUV2.x, borderUV1.y),
		    float2(borderUV1.x, borderUV2.y), float2(borderUV2.x, borderUV2.y),
        },
        {
            float2(borderUV2.x, borderUV1.y), float2(borderUV3.x, borderUV1.y),
		    float2(borderUV2.x, borderUV2.y), float2(borderUV3.x, borderUV2.y),
        },
        
        // --------- 20 - 22 --------- //
        {
            float2(borderUV0.x, borderUV2.y), float2(borderUV1.x, borderUV2.y),
		    float2(borderUV0.x, borderUV3.y), float2(borderUV1.x, borderUV3.y),
        },
        {
            float2(borderUV1.x, borderUV2.y), float2(borderUV2.x, borderUV2.y),
		    float2(borderUV1.x, borderUV3.y), float2(borderUV2.x, borderUV3.y),
        },
        {
            float2(borderUV2.x, borderUV2.y), float2(borderUV3.x, borderUV2.y),
		    float2(borderUV2.x, borderUV3.y), float2(borderUV3.x, borderUV3.y),
        },
    };
    
    // ---------- �]�� ---------- //
    
    // �s���䗦���X�P�[���k��
    float2 scaleRatio = 1.0f / texRange * texSizeRatio;
    matrix mAdjustScale =
    {
        scaleRatio.x, 0.0f, 0.0f, 0.0f,
		0.0f, scaleRatio.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // �s����|�����킹��
    matrix mWorld = mul(matWorld, mAdjustScale);
    
	// �s��������ăs�N�Z���V�F�[�_�[�ɓ]��
    for (uint i = 0; i < sectionNum; i++)
    {
        for (uint j = 0; j < vertexNum; j++)
        {
            float4 local = input[0].pos + float4(posLT + offsets[i][j], 0, 0);
            element.svpos = mul(mWorld, local);
            element.uv = uvLT + uvs[i][j];
            output.Append(element);
        }
    }
}