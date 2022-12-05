#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"

class Sprite
{
public:
	//初期化
	void Initialize(SpriteCommon* spriteCommon_);
	//更新
	void Draw();

private:
	//スプライト
	SpriteCommon* spriteCommon = nullptr;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

