#include <cassert>
#include "Util.h"
#include "Pipeline.h"

RDirectX* Pipeline::directX_ = RDirectX::GetInstance();

RDirectX* ParticlePipeline::directX_ = RDirectX::GetInstance();

D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC()
{
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A�ۊ�
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	return samplerDesc;
}

void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, int blend)
{

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	//���ʐݒ�i�A���t�@�l�j
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��
	//���Z����
	if (blend == BlendNum::ADD) {
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	//���Z����
	else if (blend == BlendNum::SUB) {
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	//�F���]
	else if (blend == BlendNum::NEGA) {
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	//����������
	else if (blend == BlendNum::ALPHA) {
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}
}

void PipelineObject::Create(BlendNum blendNum, CULL_MODE cullmode, TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc)
{
	HRESULT result;

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �V�F�[�_�[�̐ݒ�
	if (vsBlob != nullptr) {
		pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	}
	if (psBlob != nullptr) {
		pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	}
	if (gsBlob != nullptr) {
		pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
	}

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A�ۊ�
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	//���[�g�p�����[�^�ݒ�
	AddrootParams();

	// ���[�g�V�O�l�`��
	// // ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	// ���[�g�V�O�l�`���̐ݒ�
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams_.data();	//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = (UINT)rootParams_.size();			//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = RDirectX::GetInstance()->GetDevice()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE(cullmode); // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(depthWriteMasc);//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE(topologytype);
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �u�����h�X�e�[�g
	SetBlend(pipelineDesc, blendNum);
	// �p�C�v�����X�e�[�g�̐���
	if (blendNum == BlendNum::ADD) {
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateAdd));
	}else if (blendNum == BlendNum::NEGA) {
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateNega));
	}
	else if (blendNum == BlendNum::SUB) {
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateSub));
	}
	else if (blendNum == BlendNum::ALPHA) {
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateAlpha));
	}
	assert(SUCCEEDED(result));
}

void PipelineObject::Setshader(LPCWSTR fileName, ShaderType shadertype)
{
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	if (shadertype == ShaderType::VS) {
		ShaderCompileFromFile(fileName, "main", "vs_5_0", &vsBlob, errorBlob.Get());
	}
	else if (shadertype == ShaderType::PS) {
		ShaderCompileFromFile(fileName, "main", "ps_5_0", &psBlob, errorBlob.Get());
	}
	else if (shadertype == ShaderType::GS) {
		ShaderCompileFromFile(fileName, "main", "gs_5_0", &gsBlob, errorBlob.Get());
	}
}

void PipelineObject::AddrootParams()
{
	rootParams_.clear();
	uint32_t size = 0;
	//inputLayout + 1�̐���rootParams_�����
	for (int i = 0; i < inputLayout.size() + 1; i++) {
		D3D12_ROOT_PARAMETER rootParams{};
		//�z��̍ŏ��Ƀe�N�X�`��
		if (rootParams_.size() == 0) {
			//�f�X�N���v�^�����W�̐ݒ�
			D3D12_DESCRIPTOR_RANGE* descriptorRange = 
				new D3D12_DESCRIPTOR_RANGE{};

			//�f�X�N���v�^�����W�̐ݒ�
			descriptorRange->NumDescriptors = 1;					//��x�̕`��ɍD�����e�N�X�`����1���Ȃ̂�1
			descriptorRange->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			descriptorRange->BaseShaderRegister = 0;				//�e�N�X�`�����W�X�^0��
			descriptorRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
			rootParams.DescriptorTable.pDescriptorRanges = descriptorRange;//�f�X�N���v�^�����W
			rootParams.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
		}
		else {
			//�萔�o�b�t�@
			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
			rootParams.Descriptor.ShaderRegister = size;					//�萔�o�b�t�@�ԍ�
			rootParams.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
			size++;
		}

		rootParams_.emplace_back(rootParams);
	}
}

void PipelineObject::AddInputLayout(const char* semanticName, DXGI_FORMAT format)
{
	inputLayout.push_back(
		{
		semanticName, 0, format, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		});
}
