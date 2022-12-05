#pragma once
#include<d3d12.h>
#include <wrl.h>

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
};

