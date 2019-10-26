#include <Windows.h>
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib");
#pragma comment(lib,"d3dx9.lib");


#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "姆姆游戏引擎demo"


LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;

bool InitializeD3D(HWND hWnd);
void Shutdown();
void RenderScene();

LRESULT WINAPI MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{

	//用户通过点击关闭程序按钮后，
	//消息队列增加一条消息WM_CLOSE，
	//然后程序从消息队列中取走WM_CLOSE，
	//调用DestroyWindow()，
	//消息队列增加WM_DESTROY，
	//应用程序再次取走，
	//并调用PostQuitMessage()，
	//最终得到WM_QUIT而使消息循环退出，
	//程序退出。
	case WM_DESTROY:
		PostQuitMessage(9);
		return 0;
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)PostQuitMessage(0);
		
		break;


	default:
		break;
	}

	//将消息传递给window去处理
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
			640,
			480,
			GetDesktopWindow(),
			NULL,
			hInst,
			NULL);

	if (InitializeD3D(hWnd))
	{

		ShowWindow(hWnd,SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		//清空
		ZeroMemory(&msg,sizeof(msg));

		//消息循环
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
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
	return true;

}


void Shutdown()
{
	if (g_D3DDevice != NULL)g_D3DDevice->Release();
	if (g_D3D != NULL)g_D3D->Release();
}


void RenderScene()
{
	g_D3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0F,0);
	g_D3DDevice->BeginScene();
	//输出3D图
	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL,NULL,NULL,NULL);
}