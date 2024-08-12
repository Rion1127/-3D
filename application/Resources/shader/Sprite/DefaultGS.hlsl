#include "Default.hlsli"

static const uint vNum = 4;

[maxvertexcount(vNum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
	// ----- 左右上下位置 ----- //
	
    float left   = (0.0f - texAnchor.x) * texRange.x * texFlipX;
    float right  = (1.0f - texAnchor.x) * texRange.x * texFlipX;
    float top    = (0.0f - texAnchor.y) * texRange.y * texFlipY;
    float bottom = (1.0f - texAnchor.y) * texRange.y * texFlipY;

	// -------- UV座標 -------- //
    
    // 空白
    float2 blank = (texSeqSize - texRange) * texAnchor;
    
    // 左上 = (一枚の大きさ * 何番目) + 空白分
	// 右下 = 左上 + 絵の大きさ
    float2 texLT = (texSeqSize * texSeqNum) + blank;
    float2 texRB = texLT + texRange;
    
    // 全体からの割合でUV値を算出
    float2 texSize = texSeqSize * texSeqMax;
    
    float uvLeft   = texLT.x / texSize.x;
    float uvRight  = texRB.x / texSize.x;
    float uvTop    = texLT.y / texSize.y;
    float uvBottom = texRB.y / texSize.y;

	// -------- 配列化 --------//
    
    float4 offsets[vNum] =
    {
        float4( left, bottom, 0, 0), // 左下
		float4( left,    top, 0, 0), // 左上
		float4(right, bottom, 0, 0), // 右下
		float4(right,    top, 0, 0), // 右上
    };
    float2 uvs[vNum] =
    {
        float2( uvLeft, uvBottom), // 左下
		float2( uvLeft,    uvTop), // 左上
		float2(uvRight, uvBottom), // 右下
		float2(uvRight,    uvTop), // 右上
    };
    
    // --------- 転送 --------- //
    
    // 行列を比率分スケール縮小
    float2 scaleRatio = 1.0f / texRange * texSizeRatio;
    matrix mAdjustScale =
    {
        scaleRatio.x, 0.0f, 0.0f, 0.0f,
		0.0f, scaleRatio.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // 行列を掛け合わせる
    matrix mWorld = mul(matWorld, mAdjustScale);
    
	// 行列をかけてピクセルシェーダーに転送
    for (uint i = 0; i < vNum; i++)
    {
        float4 local = input[0].pos + offsets[i];
        element.svpos = mul(mWorld, local);
        element.uv = uvs[i];
        output.Append(element);
    }
}