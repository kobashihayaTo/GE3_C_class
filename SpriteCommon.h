#pragma once
#include<d3d12.h>
#include <wrl.h>

#include "DirectXcommon.h"

class SpriteCommon
{
public://�����o�֐�
	//������
	void Initialize(DirectXcommon*dxCommon_);

	//�`��
	void PreDraw(); 

	//Getter
	DirectXcommon* GetDirectXCommon() { return dxCommon; }

private:
	DirectXcommon* dxCommon;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//���[�v�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
};

