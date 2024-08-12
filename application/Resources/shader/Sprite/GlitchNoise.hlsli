// ノイズ設定
cbuffer cbGlitchNoise : register(b3)
{
    float time; // 時間
    
    float glitchIntensity; // グリッチの強さ
    
    float blockScale; // ブロックノイズのブロックの大きさ
    
    float noiseSpeed; // ノイズの速さ
}