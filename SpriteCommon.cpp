#include "SpriteCommon.h"

#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

void SpriteCommon::Initialize(DirectXcommon* dxCommon_)
{
	HRESULT result{};
	assert(dxCommon_);
	dxCommon = dxCommon_;

    imageData=new XMFLOAT4[imageDataCount];
    for (size_t  i = 0; i < imageDataCount; i++)
    {
        imageData[i].x = 1.0f;
        imageData[i].y = 0.0f;
        imageData[i].z = 0.0f;
        imageData[i].w = 1.0f;
    }
    //テクスチャ
    {
        D3D12_HEAP_PROPERTIES cbHeapProp{};
        cbHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
        cbHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        cbHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

        D3D12_RESOURCE_DESC textureResourceDesc{};
        textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureResourceDesc.Width = textureWidth;
        textureResourceDesc.Height= textureHeight;
        textureResourceDesc.DepthOrArraySize = 1;
        textureResourceDesc.MipLevels = 1;
        textureResourceDesc.SampleDesc.Count = 1;

        // 定数バッファの生成
        result = dxCommon->GetDevice()->CreateCommittedResource(
            &cbHeapProp, // ヒープ設定
            D3D12_HEAP_FLAG_NONE,
            &textureResourceDesc, // リソース設定
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&texBuff)
        );

        result = texBuff->WriteToSubresource(
            0,
            nullptr,//全領域へコピー
            imageData,//元データアドレス
            sizeof(XMFLOAT4) * textureWidth,//1ラインサイズ
            sizeof(XMFLOAT4) * imageDataCount//全サイズ
        );

        delete[] imageData;
    }

    //SRVの最大個数
    const size_t kMaxSRVCount = 2056;

    //デスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvHeapDesc.NumDescriptors = kMaxSRVCount;

    //設定
    result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));

    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    //ハンドルの指す位置にシェーダーリソースビュー作成
    dxCommon->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	//シェーダー
    ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob;// エラーオブジェクト

    // 頂点シェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/shaders/SpriteVS.hlsl",  // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &vsBlob, &errorBlob
    );
    // エラーなら
    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    // ピクセルシェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/shaders/SpritePS.hlsl",   // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &psBlob, &errorBlob
    );
    // エラーなら
    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    // 頂点レイアウト
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz座標(1行で書いたほうが見やすい)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // uv座標
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };

    // グラフィックスパイプライン設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
    // シェーダーの設定
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
    // サンプルマスクの設定
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
    // ラスタライザの設定
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
    pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
    // ブレンドステート
    pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
    // 頂点レイアウトの設定
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
    // 図形の形状設定
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // その他の設定
    pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
    pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

    //デスクリプタレンジ
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //ルートパラメーター
    D3D12_ROOT_PARAMETER rootParams[2] = {};
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParams[0].Descriptor.ShaderRegister = 0;
    rootParams[0].Descriptor.RegisterSpace = 0;
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    //テクスチャレジスタ
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // テクスチャサンプラーの設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //横繰り返し（タイリング）
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //縦繰り返し（タイリング）
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //奥行繰り返し（タイリング）
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //ボーダーの時は黒
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //全てリニア補間
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //ミップマップ最大値
    samplerDesc.MinLOD = 0.0f;                                              //ミップマップ最小値
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // ルートシグネチャ
    // ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;
    rootSignatureDesc.NumParameters = _countof(rootParams);
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    // ルートシグネチャのシリアライズ
    ID3DBlob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(result));
    result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));
    rootSigBlob->Release();
    // パイプラインにルートシグネチャをセット
    pipelineDesc.pRootSignature = rootSignature.Get();
    // パイプランステートの生成

    result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));

}

void SpriteCommon::PreDraw()
{
    //プリミティブ形状の設定コマンド
    dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //パイプラインステートとルートシグネチャの設定
    dxCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
    dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

    //デスクリプタ
    ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
    dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);

    //SRVヒープ
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
    dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

}
