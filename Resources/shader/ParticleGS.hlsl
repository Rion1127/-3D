#include "Particle.hlsli"

[maxvertexcount(3)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	//for (uint i = 0; i < 3; i++)
	{
        GSOutput element;
        element.normal = input[0].normal;
        element.uv = input[0].uv;
        
        element.svpos = input[0].svpos;
        output.Append(element);
        element.svpos = input[0].svpos + float4(10.f, 10.f, 0, 0);
        output.Append(element);
        element.svpos = input[0].svpos + float4(10.f, 0.f, 0, 0);
        output.Append(element);
    }
}