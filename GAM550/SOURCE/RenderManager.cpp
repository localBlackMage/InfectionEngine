#include "Stdafx.h"

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		INFECT_THREAD_JOBS.AddNewJob(new RenderTerminateTerminate(*INFECT_THREAD_JOBS.GetThreadContainer<RenderThreadContainer>(ThreadType::RenderThread)));
		INFECT_THREAD_JOBS.AddNewJob(new SimulationTerminateTerminate(*INFECT_THREAD_JOBS.GetThreadContainer<SimulationThreadContainer>(ThreadType::SimThread)));
		INFECT_GAME_STATE.SetGameState(GameState::QUIT);
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool RenderManager::_GameObjectHasRenderableComponent(const GameObject & gameObject)
{
	return gameObject.HasComponent(ComponentType::C_Mesh);
}

RenderManager::RenderManager() :
	m_ClearColor(Color(0.1f, 0.1f, 0.1f, 1)),
	mp_D3D(new D3DHandler())
{
}

RenderManager::~RenderManager()
{
	if (mp_VS)
		mp_VS->Release();
	if (mp_PS)
		mp_PS->Release();
	if (mp_Cbuffer)
		mp_Cbuffer->Release();

	// Show the mouse cursor.
	ShowCursor(true);
	delete mp_D3D;
}


void RenderManager::InitConsole()
{
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Infect Engine");
	}
}

void RenderManager::DestroyConsole() 
{
	if (FreeConsole()) {
		
	}
}

bool RenderManager::InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings)
{
	m_WindowSettings = settings;
	m_hWnd = mp_D3D->InitWindow(hInstance, nCmdShow, settings);
	return mp_D3D->InitD3D(m_hWnd, settings);
}

void RenderManager::BindBackBuffer()
{
	mp_D3D->BindBackBuffer();
}

void RenderManager::ClearScreen(void)
{
	// Clear the back buffer and then the depth buffer
	mp_D3D->ClearBackBuffer(m_ClearColor);
	mp_D3D->ClearDepthBuffer();
}

void RenderManager::PresentFrameToScreen(void)
{
	mp_D3D->PresentBuffer(m_WindowSettings.VSync);
}

void RenderManager::RenderObject(const GameObject& pGOCamera, const GameObject& pGO) 
{
	if (!_GameObjectHasRenderableComponent(pGO))
		return;

	const CameraComponent * pCamComp = pGOCamera.GetComponent<CameraComponent>();
	const MeshComponent * pMeshComp = pGO.GetComponent<MeshComponent>();

	Matrix4x4 M = pGO.GetComponent<TransformComponent>()->GetTransform();
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	Matrix4x4 I = Matrix4x4::Transpose(N) * M;
	ConstantBuffer cb;
	cb.MatFinal = pCamComp->GetCameraMatrix() * pCamComp->GetViewMatrix() * M;
	cb.MatFinal.Transpose();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.NormalMatrix = Matrix4x4::Transpose(N);
	cb.CastShadows = pMeshComp->CastShadows();
	cb.ReceiveShadows = pMeshComp->ReceiveShadows();
	cb.IsLit = pMeshComp->IsLit();
	cb.Textured = pMeshComp->IsTextured();
	cb.CameraPosition = pGOCamera.GetComponent<TransformComponent>()->WorldPosition();// Vector3D(camPos.w, camPos.z, camPos.y, camPos.x);
	// TODO: THIS IS A HACK, REMOVE IT
	cb.LightPosition = INFECT_GOM.GetGameObject(3)->GetComponent<TransformComponent>()->WorldPosition();

	mp_D3D->mp_DeviceContext->VSSetConstantBuffers(0, 1, &mp_Cbuffer);
	mp_D3D->mp_DeviceContext->PSSetConstantBuffers(0, 1, &mp_Cbuffer);
	ID3D11ShaderResourceView* ptex = pMeshComp->GetDiffuseTexture();
	mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &ptex);

	// set the new values for the constant buffer
	mp_D3D->mp_DeviceContext->UpdateSubresource(mp_Cbuffer, 0, 0, &cb, 0, 0);

	// do 3D rendering on the back buffer here
	RenderScene(pMeshComp->GetScene());
}

void RenderManager::RenderScene(const Scene * pScene)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (int i = 0; i < pScene->NumMeshes(); ++i) {
		const Mesh* pMesh = (*pScene)[i];
		ID3D11Buffer* buffers[] = { pMesh->VBuffer() };
		mp_D3D->mp_DeviceContext->IASetVertexBuffers(0, 1, &(buffers[0]), &stride, &offset);
		mp_D3D->mp_DeviceContext->IASetIndexBuffer(pMesh->IBuffer(), DXGI_FORMAT_R32_UINT, 0);

		mp_D3D->mp_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mp_D3D->mp_DeviceContext->DrawIndexed(pMesh->NumFaces()*3, 0, 0);
	}
}

bool RenderManager::LoadShader()
{
	// load and compile the shaders
	int flag = D3D10_SHADER_WARNINGS_ARE_ERRORS;// | D3D10_SHADER_OPTIMIZATION_LEVEL3;

	HRESULT result = D3DCompileFromFile(L"ASSETS/SHADERS/base3D.shader", 0, 0, "VShader", "vs_4_0", flag, flag, &mp_VSBlob, &mp_Errors);
	//D3DX11CompileFromFile("ASSETS/SHADERS/base3D.shader", 0, 0, "VShader", "vs_4_0", flag, 0, 0, &mp_VSBlob, &mp_Errors, 0);
	if (FAILED(result)) {
		MessageBox(NULL, "The vertex shader failed to compile.", "Error", MB_OK);
		return false;
	}

	result = D3DCompileFromFile(L"ASSETS/SHADERS/base3D.shader", 0, 0, "PShader", "ps_4_0", flag, flag, &mp_PSBlob, &mp_Errors);
	//D3DX11CompileFromFile("ASSETS/SHADERS/base3D.shader", 0, 0, "PShader", "ps_4_0", flag, 0, 0, &mp_PSBlob, &mp_Errors, 0);
	if (FAILED(result)) {
		MessageBox(NULL, "The pixel shader failed to compile.", "Error", MB_OK);
		return false;
	}

	// Encapsulate both shaders into shader objects
	mp_D3D->mp_Device->CreateVertexShader(mp_VSBlob->GetBufferPointer(), mp_VSBlob->GetBufferSize(), NULL, &mp_VS);
	mp_D3D->mp_Device->CreatePixelShader(mp_PSBlob->GetBufferPointer(), mp_PSBlob->GetBufferSize(), NULL, &mp_PS);

	mp_D3D->mp_DeviceContext->VSSetShader(mp_VS, 0, 0);
	mp_D3D->mp_DeviceContext->PSSetShader(mp_PS, 0, 0);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer) + (16 - (sizeof(ConstantBuffer) % 16));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	mp_D3D->mp_Device->CreateBuffer(&bd, NULL, &mp_Cbuffer);

	return true;
}
