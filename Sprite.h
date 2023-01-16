#pragma once
#include "SpriteCommon.h"

class Sprite
{
public:
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

	DirectX::XMFLOAT4 color = { 1,0,0,0.5f };


	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

