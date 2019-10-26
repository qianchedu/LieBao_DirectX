#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//视图变换	世界变换

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "姆姆游戏引擎模板"
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

	//将消息传递给window去处理
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
		NULL,				//图标
		NULL,				//鼠标
		NULL,				//背景色//(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,				//菜单
		WINDOW_CLASS,			//窗口类名称
		NULL				//创建窗口的
	};

	//注册
	RegisterClassEx(&wc);

	//创建窗口类 
	HWND hWnd = CreateWindow(WINDOW_CLASS,			//参数1：创建的窗口类名称
		WINDOW_TITLE,						//参数2：窗口类的标题
		WS_OVERLAPPEDWINDOW,				//参数3：窗口的样式
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
		//清空
		ZeroMemory(&msg, sizeof(msg));

		//消息循环
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//没有消息要处理，就可以做其他工作；
				RenderScene();
			}
		}
	}

	Shutdown();
	//取消注册-释放资源
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

	//如果是全屏
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
	

	//要绘制的d3d对象
	if (!InitializeObjects())
		return false;
	return true;

}


//初始化要绘制的对象
bool InitializeObjects()
{

	//透视投影
	D3DXMatrixPerspectiveFovLH(&g_projection, 45.0f, WINDOW_WIDTH / WIDNOW_HEIGHT, 0.1F, 1000.0F);
	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);
	//关闭灯光
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//摄像机的位置
	D3DXVECTOR3 cameraPos(0.0f, 0.0f, -1.0f);
	//摄像机的镜头指向原点
	D3DXVECTOR3 lookAtPos(0.0f, 0.0f, 0.0f);
	//摄像机的上方
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos, &lookAtPos, &upDir);
	return true;
}


void RenderScene()
{
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);
	g_D3DDevice->BeginScene();

	//视图变换
	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);

	//输出3D图

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


//绘制流水线
//本地坐标系	世界坐标系	观察坐标系	背面消隐	关照
//裁剪			投影		视口坐标系	光栅化



