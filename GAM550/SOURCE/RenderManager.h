/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

// 208 bytes
struct ConstantBuffer {
	Matrix4x4 MatFinal;
	Matrix4x4 ModelMatrix;
	Matrix4x4 NormalMatrix;
	Vector3D CameraPosition;
};

class RenderManager : public Subscriber
{
private:
	IDXGISwapChain *mp_SwapChain;
	ID3D11Device *mp_Device;
	ID3D11DeviceContext *mp_DeviceContext;
	ID3D11RenderTargetView *mp_BackBuffer, *mp_DepthBuffer;

	ID3D11Texture2D* mp_DepthStencilBuffer;
	ID3D11DepthStencilState* mp_DepthStencilState;
	ID3D11DepthStencilView* mp_DepthStencilView;
	ID3D11RasterizerState* mp_RasterState;
	int m_VideoCardMemory;
	char m_VideoCardDescription[128];

	D3DXCOLOR m_ClearColor;
	unsigned int m_ScreenWidth, m_ScreenHeight;
	float m_AspectRatio;
	bool m_FullScreen;
	bool m_VSyncEnabled;
	HWND m_hWnd; 	// the handle for the window, filled by a function

	ID3D11Buffer *mp_Cbuffer;		// the constant buffer
	ID3D11VertexShader *mp_VS;		// the vertex shader
	ID3D11PixelShader *mp_PS;		// the pixel shader
	ID3D10Blob *mp_VSBlob, *mp_PSBlob, *mp_Errors;

	bool _GameObjectHasRenderableComponent(const GameObject& gameObject);
public:
	RenderManager();
	~RenderManager();

	inline IDXGISwapChain * const SwapChain() { return mp_SwapChain; }
	inline ID3D11Device * const Device() { return mp_Device; }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_DeviceContext; }
	inline ID3D11RenderTargetView * const BackBuffer() { return mp_BackBuffer; }
	inline ID3D11RenderTargetView * const DepthBuffer() { return mp_BackBuffer; }

	inline float AspectRatio() const { return m_AspectRatio; }
	inline unsigned int ScreenWidth() const { return m_ScreenWidth; }
	inline unsigned int ScreenHeight() const { return m_ScreenHeight; }

	// Creates a console for output
	void InitConsole();
	// Destroys the active console
	void DestroyConsole();
	// Sets up and initializes window
	void InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings);
	// Sets up and initializes Direct3D
	bool InitD3D(HWND hWnd, WindowSettings settings);
	// Closes Direct3D and releases memory
	void CleanD3D(void);

	void FrameStart(void);

	void FrameEnd(void);

	// Renders an object given a specific camera
	void RenderObject(const GameObject& pGOCamera, const GameObject& pGO);

	void RenderScene(const Scene * pScene);

	void LoadShader(void);

	HWND GethWnd() { return m_hWnd; }

	// TODO: Get rid of these in favor of selecting which VS/PS you want
	ID3D11VertexShader * const VertexShader() { return mp_VS; }
	ID3D11PixelShader * const PixelShader() { return mp_PS; }
	ID3D10Blob * const VSBlob() { return mp_VSBlob; }
	ID3D10Blob * const PSBlob() { return mp_PSBlob; }
};

#endif

