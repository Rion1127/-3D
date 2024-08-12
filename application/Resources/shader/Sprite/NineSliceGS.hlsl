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
    
    // テクスチャサイズが一定になるようにUVボーダーを補正する
    // UV : 補正後UV = 1 : 割合 (テクスチャ範囲 / スケール)
    float2 adjustedUVBorder = uvBorder * (texRange / scale);
    
    // ---------- 位置 ---------- //
    
    // 位置左上
    float2 posLT = texRange * -texAnchor * float2(texFlipX, texFlipY);
    
    // 位置範囲
    float2 posRange = texRange * float2(texFlipX, texFlipY);
    
    // 区切りの境界線の位置
    // (始端 | 1 | 2 | 終端)
    float2 borderPos0 = float2(0, 0);
    float2 borderPos1 = posRange * adjustedUVBorder;
    float2 borderPos2 = posRange * (1.0f - adjustedUVBorder);
    float2 borderPos3 = posRange;
    
	// --------- UV座標 --------- //
    
    // uv左上 = (一枚の大きさ * 何番目) + 空白分 / 全体の大きさ
    float2 uvLT = (texSeqSize * texSeqNum) + ((texSeqSize - texRange) * texAnchor) / (texSeqSize * texSeqMax);
    
    // uv範囲
    float2 uvRange = (texRange / texSeqSize);
    
    // 区切りの境界線のUV
    // (始端 | 1 | 2 | 終端)
    float2 borderUV0 = float2(0, 0);
    float2 borderUV1 = uvRange * uvBorder;
    float2 borderUV2 = uvRange * (1.0f - uvBorder);
    float2 borderUV3 = uvRange;
    
    // --------- 配列化 --------- //
    
	// オフセット値
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
    
    // UV値
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
    
    // ---------- 転送 ---------- //
    
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