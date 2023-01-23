#pragma once
#include<d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>

#include "DirectXcommon.h"

class SpriteCommon
{

public://メンバ関数
	//初期化
	void Initialize(DirectXcommon*dxCommon_);

	//描画
	void PreDraw(); 

	//Getter
	DirectXcommon* GetDirectXCommon() { return dxCommon; }

private:
	DirectXcommon* dxCommon;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//ループシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

