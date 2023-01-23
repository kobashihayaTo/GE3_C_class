#pragma once
#include "SpriteCommon.h"

class Sprite
{
private:
	const int window_width = 1280;
	const int window_height = 720;

	//���_�f�[�^�\����
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//�萔�o�b�t�@(�}�e���A��)
	struct  ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct ConstBufferDataTransform
	{
		DirectX::XMMATRIX mat;
	};

public:
	//������
	void Initialize(SpriteCommon* spriteCommon_);
	//�X�V
	void Draw();

private:
	//�X�v���C�g
	SpriteCommon* spriteCommon = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1.f };

	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource>constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

