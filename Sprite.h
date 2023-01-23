#pragma once
#include "SpriteCommon.h"

class Sprite
{
private:
	const int window_width = 1280;
	const int window_height = 720;

	//頂点データ構造体
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//定数バッファ(マテリアル)
	struct  ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct ConstBufferDataTransform
	{
		DirectX::XMMATRIX mat;
	};

public:
	//初期化
	void Initialize(SpriteCommon* spriteCommon_);
	//更新
	void Draw();

private:
	//スプライト
	SpriteCommon* spriteCommon = nullptr;

	DirectX::XMFLOAT4 color = { 1,1,1,1.f };

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource>constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

