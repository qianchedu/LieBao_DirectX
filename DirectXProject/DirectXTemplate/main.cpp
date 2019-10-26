#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//��ͼ�任	����任

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "ķķ��Ϸ����ģ��"
#define WINDOW_WIDTH 640
#define WIDNOW_HEIGHT 480

bool InitializeD3D(HWND hWnd,bool fullscreen);
bool InitializeObjects();
void Shutdown();
void RenderScene();


LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
D3DXMATRIX g_projection;
D3DXMATRIX g_ViewMatrix;
D3DXMATRIX g_WorldMatrix;


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)PostQuitMessage(0);

		break;


	default:
		break;
	}

	//����Ϣ���ݸ�windowȥ����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE prevhInst,
	LPTSTR cmdLine,
	int show)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		hInst,
		NULL,				//ͼ��
		NULL,				//���
		NULL,				//����ɫ//(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,				//�˵�
		WINDOW_CLASS,			//����������
		NULL				//�������ڵ�
	};

	//ע��
	RegisterClassEx(&wc);

	//���������� 
	HWND hWnd = CreateWindow(WINDOW_CLASS,			//����1�������Ĵ���������
		WINDOW_TITLE,						//����2��������ı���
		WS_OVERLAPPEDWINDOW,				//����3�����ڵ���ʽ
		100,
		100,
		WINDOW_WIDTH,
		WIDNOW_HEIGHT,
		GetDesktopWindow(),
		NULL,
		hInst,
		NULL);

	if (InitializeD3D(hWnd,false))
	{

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		//���
		ZeroMemory(&msg, sizeof(msg));

		//��Ϣѭ��
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//û����ϢҪ�����Ϳ���������������
				RenderScene();
			}
		}
	}

	Shutdown();
	//ȡ��ע��-�ͷ���Դ
	UnregisterClass(WINDOW_CLASS, hInst);
	return 0;
}



bool InitializeD3D(HWND hWnd,bool fullscreen)
{
	D3DDISPLAYMODE displayMode;
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == NULL)return false;
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//�����ȫ��
	if (fullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WIDNOW_HEIGHT;
	}
	else 
	{
		d3dpp.Windowed = TRUE;
	}

	
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_D3DDevice)))
	{
		return false;
	}
	

	//Ҫ���Ƶ�d3d����
	if (!InitializeObjects())
		return false;
	return true;

}


//��ʼ��Ҫ���ƵĶ���
bool InitializeObjects()
{

	//͸��ͶӰ
	D3DXMatrixPerspectiveFovLH(&g_projection, 45.0f, WINDOW_WIDTH / WIDNOW_HEIGHT, 0.1F, 1000.0F);
	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);
	//�رյƹ�
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//�������λ��
	D3DXVECTOR3 cameraPos(0.0f, 0.0f, -1.0f);
	//������ľ�ͷָ��ԭ��
	D3DXVECTOR3 lookAtPos(0.0f, 0.0f, 0.0f);
	//��������Ϸ�
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos, &lookAtPos, &upDir);
	return true;
}


void RenderScene()
{
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);
	g_D3DDevice->BeginScene();

	//��ͼ�任
	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);

	//���3Dͼ

	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}



void Shutdown()
{
	if (g_D3DDevice != NULL)g_D3DDevice->Release();
	if (g_D3D != NULL)g_D3D->Release();

	g_D3DDevice = NULL;
	g_D3D = NULL;
}


//������ˮ��
//��������ϵ	��������ϵ	�۲�����ϵ	��������	����
//�ü�			ͶӰ		�ӿ�����ϵ	��դ��



