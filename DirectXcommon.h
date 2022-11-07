#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

#include "WinApp.h"

class DirectXcommon
{
public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp);

	/// <summary>
	///描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	///	描画後処理
	/// </summary>
	void PostDraw();

	//デバイス取得
	ID3D12Device* GetDevice() const { return device.Get(); }

	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
private:
	/// <summary>
	/// デバイス初期化
	/// </summary>
	void InitializeDevice();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーン初期化
	/// </summary>
	void InitializeSwapchain();

	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void InitializeRenderTargetView();
	/// <summary>
	/// 深度バッファ
	/// </summary>
	void InitializeDepthBuffer();

	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void InitializeFence();


private:
	WinApp* winApp = nullptr;

	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	//コマンド関連
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	//スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
};

