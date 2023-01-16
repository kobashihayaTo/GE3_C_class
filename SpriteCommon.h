#pragma once
#include<d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>

#include "DirectXcommon.h"

class SpriteCommon
{
public:
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;
	const size_t imageDataCount = textureWidth * textureHeight;

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

	//�摜
	DirectX::XMFLOAT4* imageData ;
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

