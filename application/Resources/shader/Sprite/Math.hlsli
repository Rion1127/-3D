
// ƒ‰ƒ“ƒ_ƒ€’l
float Random(float2 seeds)
{
    return frac(sin(dot(seeds, float2(12.9898, 78.233))) * 43758.5453);
}