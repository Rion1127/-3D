#include "Particle.hlsli"


VSOutput main(float4 pos : POSITION)
{
	//ピクセルシェーダーに渡す値
    VSOutput output;
    output.pos = pos;
    return output;
}