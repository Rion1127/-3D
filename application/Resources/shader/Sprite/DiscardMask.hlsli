#include "Default.hlsli"

// マスク設定
cbuffer cbMask : register(b3)
{
    float maskRate;
}