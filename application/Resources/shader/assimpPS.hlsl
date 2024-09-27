#include "assimp.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    if (m_isUseBaseColor)
    {
        texcolor.rgb *= (m_basecolor.rgb / 255.f);
    }
    //float3 light = normalize(float3(1, -1, 1));
    //float diffuse = saturate(dot(-light, input.normal));
    //float brightness = diffuse + 0.3f;
    ////float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
    
    
	//����x
    const float shininess = 20.0f;
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.svpos.xyz);
	//�����ˌ�
    float3 ambient = m_ambient * 0.3f * ambientColor;
	//�V�F�[�f�B���O
    float4 shadecolor = float4(ambient, m_alpha);
    
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
            float3 diffuse = m_diffuse * intensity * dirLights[i].lightColor;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;
			//���ׂĉ��Z����
            shadecolor.rgb += (diffuse.rgb + specular.rgb) * dirLights[i].lightColor;
            //shadecolor.a = 1;
        }
    }
	//�_����
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
			//���C�g�̃x�N�g��
            float3 lightv = pointLights[i].lightpos - input.svpos.xyz;
			//�x�N�g���̒���
            float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
            lightv = normalize(lightv);
			//���������W��
            float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d +
			pointLights[i].lightAtten.z * d * d);
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;

        }
    }
	//�X�|�b�g���C�g
    for (int i = 0; i < SPOTLIGHT_NUM; i++)
    {
        if (spotLights[i].active)
        {
			//���C�g�ւ̕����x�N�g��
            float3 lightv = spotLights[i].lightpos - input.svpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
			//���������W��
            float atten =
			saturate(1.0f / (spotLights[i].lightatten.x +
			spotLights[i].lightatten.y * d +
			spotLights[i].lightatten.z * d * d));
			//�p�x����
            float cos = dot(lightv, spotLights[i].lightv);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓����͂P�{ �����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(spotLights[i].lightActorAngleCos.y,
			spotLights[i].lightActorAngleCos.x, cos);
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
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }
	//�ۉe
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
			//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
            float3 casterv = circleShadows[i].casterPos - input.svpos.xyz;
			//���e�����ł̋���
            float d = dot(casterv, circleShadows[i].dir);
			//���������W��
            float atten =
			saturate(1.0f / (circleShadows[i].atten.x +
			circleShadows[i].atten.y * d +
			circleShadows[i].atten.z * d * d));
			//�������}�C�i�X�Ȃ�0�ɂ���
            atten *= step(0, d);
			//���z���C�g�̍��W
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
            float3 lightv = normalize(lightpos - input.svpos.xyz);
			//�p�x����
            float cos = dot(lightv, circleShadows[i].dir);
			//�����J�n�e������A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{�A�����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(circleShadows[i].factorAngleCos.y,
			circleShadows[i].factorAngleCos.x, cos);
			//�p�x��������Z
            atten *= angleatten;
			//���ׂČ��Z����
            shadecolor.rgb -= atten;
        }
    }
		
    return shadecolor * (texcolor);
}