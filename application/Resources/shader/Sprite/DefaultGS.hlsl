#include "Default.hlsli"

static const uint vNum = 4;

[maxvertexcount(vNum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
	// ----- ���E�㉺�ʒu ----- //
	
    float left   = (0.0f - texAnchor.x) * texRange.x * texFlipX;
    float right  = (1.0f - texAnchor.x) * texRange.x * texFlipX;
    float top    = (0.0f - texAnchor.y) * texRange.y * texFlipY;
    float bottom = (1.0f - texAnchor.y) * texRange.y * texFlipY;

	// -------- UV���W -------- //
    
    // ��
    float2 blank = (texSeqSize - texRange) * texAnchor;
    
    // ���� = (�ꖇ�̑傫�� * ���Ԗ�) + �󔒕�
	// �E�� = ���� + �G�̑傫��
    float2 texLT = (texSeqSize * texSeqNum) + blank;
    float2 texRB = texLT + texRange;
    
    // �S�̂���̊�����UV�l���Z�o
    float2 texSize = texSeqSize * texSeqMax;
    
    float uvLeft   = texLT.x / texSize.x;
    float uvRight  = texRB.x / texSize.x;
    float uvTop    = texLT.y / texSize.y;
    float uvBottom = texRB.y / texSize.y;

	// -------- �z�� --------//
    
    float4 offsets[vNum] =
    {
        float4( left, bottom, 0, 0), // ����
		float4( left,    top, 0, 0), // ����
		float4(right, bottom, 0, 0), // �E��
		float4(right,    top, 0, 0), // �E��
    };
    float2 uvs[vNum] =
    {
        float2( uvLeft, uvBottom), // ����
		float2( uvLeft,    uvTop), // ����
		float2(uvRight, uvBottom), // �E��
		float2(uvRight,    uvTop), // �E��
    };
    
    // --------- �]�� --------- //
    
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
    for (uint i = 0; i < vNum; i++)
    {
        float4 local = input[0].pos + offsets[i];
        element.svpos = mul(mWorld, local);
        element.uv = uvs[i];
        output.Append(element);
    }
}