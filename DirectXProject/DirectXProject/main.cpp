#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "ķķ��Ϸ����demo"
#define WINDOW_WIDTH 640
#define WIDNOW_HEIGHT 480


LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
D3DXMATRIX g_ortho;
D3DXMATRIX g_projection;
LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = NULL;

bool InitializeD3D(HWND hWnd);
bool InitializeObjects();
void Shutdown();
void RenderScene();


//struct stD3DVertex
//{
//	float x, y, z, rhw;
//	unsigned long color;
//};

struct stD3DVertex
{
	float x, y, z;
	unsigned long color;
};

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


LRESULT WINAPI MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{

	//�û�ͨ������رճ���ť��
	//��Ϣ��������һ����ϢWM_CLOSE��
	//Ȼ��������Ϣ������ȡ��WM_CLOSE��
	//����DestroyWindow()��
	//��Ϣ��������WM_DESTROY��
	//Ӧ�ó����ٴ�ȡ�ߣ�
	//������PostQuitMessage()��
	//���յõ�WM_QUIT��ʹ��Ϣѭ���˳���
	//�����˳���
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
	return DefWindowProc(hWnd,msg,wParam,lParam);
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

	if (InitializeD3D(hWnd))
	{

		ShowWindow(hWnd,SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		//���
		ZeroMemory(&msg,sizeof(msg));

		//��Ϣѭ��
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
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
	UnregisterClass(WINDOW_CLASS,hInst);
}



bool InitializeD3D(HWND hWnd)
{
	D3DDISPLAYMODE displayMode;
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == NULL)return false;
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
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


void Shutdown()
{
	if (g_D3DDevice != NULL)g_D3DDevice->Release();
	if (g_D3D != NULL)g_D3D->Release();
	if (g_VertexBuffer != NULL)g_VertexBuffer->Release();

	g_D3DDevice = NULL;
	g_D3D = NULL;
	g_VertexBuffer = NULL;
}


void RenderScene()
{
	g_D3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0F,0);
	g_D3DDevice->BeginScene();
	//���3Dͼ
	g_D3DDevice->SetStreamSource(0,g_VertexBuffer,0,sizeof(stD3DVertex));
	g_D3DDevice->SetFVF(D3DFVF_VERTEX);

	//��
	//g_D3DDevice->DrawPrimitive(D3DPT_LINELIST,0,2);
	//������
	g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL,NULL,NULL,NULL);
}


//������ˮ��
//��������ϵ	��������ϵ	�۲�����ϵ	��������	����
//�ü�			ͶӰ		�ӿ�����ϵ	��դ��


//��ʼ��Ҫ���ƵĶ���
bool InitializeObjects()
{
	//unsigned long col = D3DCOLOR_XRGB(255,255,255);
	//��������
	//D3DXMatrixOrthoLH(&g_ortho,WINDOW_WIDTH,WIDNOW_HEIGHT,0.1F,1000.0F);
	D3DXMatrixPerspectiveFovLH(&g_projection,45.0f,WINDOW_WIDTH/WIDNOW_HEIGHT,0.1F,1000.0F);
	g_D3DDevice->SetTransform(D3DTS_PROJECTION,&g_projection);

	//�رյƹ�
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	//����-��������
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	//stD3DVertex objData[] =
	//{
	//	{-150.0f,-150.0f,0.1f,D3DCOLOR_XRGB(255,255,0)},
	//	{ 150.0f,-150.0f,0.1f,D3DCOLOR_XRGB(255,0,0)},
	//	{   0.0f, 150.0f,0.1f,D3DCOLOR_XRGB(0,0,255),},
	//};


	stD3DVertex objData[] =
	{
		{-0.3f,-0.3f,10.1f,D3DCOLOR_XRGB(255,255,0)},
		{ 0.3f,-0.3f,10.1f,D3DCOLOR_XRGB(255,0,0)},
		{ 0.0f, 0.3f,10.1f,D3DCOLOR_XRGB(0,0,255),},
	};



	//�������㻺��
	if (FAILED(g_D3DDevice->CreateVertexBuffer(sizeof(objData), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_VertexBuffer, NULL)))
		return false;

	void *ptr;
	if (FAILED(g_VertexBuffer->Lock(0, sizeof(objData), (void**)&ptr, 0)))
		return false;

	memcpy(ptr,objData,sizeof(objData));
	
	g_VertexBuffer->Unlock();

	return true;
}
