#include "assimp.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
	//�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
	
	
	//����x
    const float shininess = 20.0f;
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//�����ˌ�
    float3 ambient = m_ambient * 0.3f * ambientColor;
	//�V�F�[�f�B���O
    float4 shadecolor = float4(ambient, m_alpha);
    float4 color = { 1, 1, 1, 1 };
	//���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float intensity = saturate(dot(normalize(input.normal), normalize(dirLights[i].lightv)));
			//���ˌ��x�N�g��
            float3 reflect = -dirLights[i].lightv + 2 * input.normal * dot(input.normal, dirLights[i].lightv);
			//�g�U���ˌ�
            float3 diffuse =  m_diffuse * intensity  * dirLights[i].lightColor;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;
			//���ׂĉ��Z����
            shadecolor.rgb += (diffuse.rgb + specular.rgb) * dirLights[i].lightColor;
            //shadecolor.a = 1;
        }
    }
	//�_����
    for (int j = 0; j < POINTLIGHT_NUM; j++)
    {
        if (pointLights[j].active)
        {
			//���C�g�̃x�N�g��
            float3 lightv = pointLights[j].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
            float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
            lightv = normalize(lightv);
			//���������W��
            float atten = 1.0f / (pointLights[j].lightAtten.x + pointLights[j].lightAtten.y * d +
			pointLights[j].lightAtten.z * d * d);
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[j].lightColor;

        }
    }
	//�X�|�b�g���C�g
    for (int k = 0; k < SPOTLIGHT_NUM; k++)
    {
        if (spotLights[k].active)
        {
			//���C�g�ւ̕����x�N�g��
            float3 lightv = spotLights[k].lightpos - input.worldpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
			//���������W��
            float atten =
			saturate(1.0f / (spotLights[k].lightatten.x +
			spotLights[k].lightatten.y * d +
			spotLights[k].lightatten.z * d * d));
			//�p�x����
            float cos = dot(lightv, spotLights[k].lightv);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓����͂P�{ �����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(spotLights[k].lightActorAngleCos.y,
			spotLights[k].lightActorAngleCos.x, cos);
			//�p�x��������Z
            atten *= angleatten;
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[k].lightcolor;
        }
    }
	//�ۉe
    for (int l = 0; l < CIRCLESHADOW_NUM; l++)
    {
        if (circleShadows[l].active)
        {
			//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
            float3 casterv = circleShadows[l].casterPos - input.worldpos.xyz;
			//���e�����ł̋���
            float d = dot(casterv, circleShadows[l].dir);
			//���������W��
            float atten =
			saturate(1.0f / (circleShadows[l].atten.x +
			circleShadows[l].atten.y * d +
			circleShadows[l].atten.z * d * d));
			//�������}�C�i�X�Ȃ�0�ɂ���
            atten *= step(0, d);
			//���z���C�g�̍��W
            float3 lightpos = circleShadows[l].casterPos + circleShadows[l].dir * circleShadows[l].distanceCasterLight;
			//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
            float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//�p�x����
            float cos = dot(lightv, circleShadows[l].dir);
			//�����J�n�e������A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{�A�����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(circleShadows[l].factorAngleCos.y,
			circleShadows[l].factorAngleCos.x, cos);
			//�p�x��������Z
            atten *= angleatten;
			//���ׂČ��Z����
            shadecolor.rgb -= atten;
        }
    }
		
    output.target0 = shadecolor * texcolor;
    output.target1 = float4(1 - (shadecolor * texcolor).rgb,1);
	//�V�F�[�f�B���O�ɂ��F�ŕ`��
    return output;
	
    //return float4(input.uv.xy, 1, 1);
    //return float4(tex.Sample(smp, input.uv));
}